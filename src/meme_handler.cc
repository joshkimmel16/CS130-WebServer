//A handler for HTTP requests that creates and serves new memes

#include "meme_handler.h"

//overriden constructor takes a config
//for this route, a config is required that contains a path to the static images
meme_handler::meme_handler (std::shared_ptr<NginxConfig> config, std::string root_path) : route_handler(config, root_path) 
{
    parse_config();
}

//overriden factory method to create a new instance
std::shared_ptr<route_handler> meme_handler::create_handler (std::shared_ptr<NginxConfig> config, std::string root_path)
{
    return std::shared_ptr<meme_handler>(new meme_handler(config, root_path));
}

//overridden method in parent class to handle a request
std::shared_ptr<response> meme_handler::handle_request (std::shared_ptr<request> req)
{
    std::string method = req->get_method();
    if (method == "POST")
    {
        return create_meme(req);
    }
    else if (method == "GET")
    {
        //check the URI to determine whether to serve the entire meme list or just a single meme
        std::string check = check_uri(req->get_uri());
        if (check == "create" || check == "create/") { return redirect_request(req, "create"); }
        else if (check == "view" || check == "view/") { return redirect_request(req, "list"); }
        else if (check.length() > 5 && check.substr(0, 5) == "view/") { return redirect_request(req, check.substr(5, check.length() - 5)); }
        else if (check == "" || check == "/") { return meme_list(req); }
        else { return get_meme_by_id(req, check); }
    }
    else
    {
        return invalid_method(req);
    }
}

//return the appropriate MIME type given a file extension
std::string meme_handler::get_mime_type (std::string extension)
{
    //mime_types is defined in mime_types.h
    std::unordered_map<std::string, std::string>::const_iterator found = mime_types.find(extension);
    if (!(found == mime_types.end()))
    {
        return found->second;
    }
    else
    {
        //this is a catch-all MIME type in case the requested extension isn't currently supported
        return "application/octet-stream";
    }   
}

//create a meme
std::shared_ptr<response> meme_handler::create_meme (std::shared_ptr<request> req)
{
    //check body of the request for validity
    std::vector<std::string> params;
    bool check = body_check(req->get_body(), params);
    
    if (!check)
    {
        //if body is not valid, return 400
        std::shared_ptr<response> resp(new response(400, "The provided meme creation request is invalid!\n"));
        resp->set_header("Content-Type", "text/plain");
        return resp;
    }
    
    //if valid, create meme using info in the body and store (???)
    std::string new_id;
    bool create = generate_new_meme(params, new_id);
    
    if (create)
    {
        //if creation successful, respond with 201
        std::shared_ptr<response> resp(new response(201, "{\"id\": \"" + new_id + "\"}"));
        resp->set_header("Content-Type", "application/json");
        return resp;
    }
    else
    {
        //if creation unsuccessful, respond with 500
        std::shared_ptr<response> resp(new response(500, "An error occurred while trying to create the provided meme!\n"));
        resp->set_header("Content-Type", "text/plain");
        return resp;
    }
}

//serve web page with list of all memes (and URL's to view them)
//TODO: is this a correct 303 response?
std::shared_ptr<response> meme_handler::redirect_request (std::shared_ptr<request> req, std::string location)
{
    if (location == "create")
    {
        //redirect to creation web page
        std::shared_ptr<response> resp(new response(303, "Location: /web/memes/create.html"));
        return resp;
    }
    else if (location == "list")
    {
        //redirect to master list web page
        std::shared_ptr<response> resp(new response(303, "Location: /web/memes/list.html"));
        return resp;
    }
    else
    {
        //redirect to viewer web page
        std::string id = location.substr(5, location.length() - 5);
        std::shared_ptr<response> resp(new response(303, "Location: /web/memes/view.html?id=" + location));
        return resp;
    }
}

//return a list of all memes as a JSON array
//TODO: implement this
//Idea: Create a file for each meme - file name is ID, contains meme template + captions
//This function loops through the directory containing all of the memes and outputs an array of the file names
std::shared_ptr<response> meme_handler::meme_list (std::shared_ptr<request> req)
{
    std::string output = "[]";
    
    //return JSON array
    std::shared_ptr<response> resp(new response(200, output));
    resp->set_header("Content-Type", get_mime_type(".json"));
    return resp;
}

//return the data associated with a particular meme (template, captions)
//TODO: implement this
//Idea: Create a file for each meme - file name is ID, contains meme template + captions
//This function finds the appropriate file based on the input ID
//And returns a JSON blob with the files contents
std::shared_ptr<response> meme_handler::get_meme_by_id (std::shared_ptr<request> req, std::string id)
{
    std::string output = "{}";
    
    //return JSON array
    std::shared_ptr<response> resp(new response(200, output));
    resp->set_header("Content-Type", get_mime_type(".json"));
    return resp;
}

//the method used in the request wasn't valid so return a 405
std::shared_ptr<response> meme_handler::invalid_method (std::shared_ptr<request> req)
{
    std::shared_ptr<response> resp(new response(405, "This route only supports the HTTP GET and POST methods!"));
    resp->set_header("Content-Type", "text/plain");
    return resp;
}

//parse URI to determine whether to serve the entire meme list or a specific meme
std::string meme_handler::check_uri (std::string uri)
{
    std::regex r("\\/memes(.*)$");
    std::smatch m;
    std::regex_search(uri, m, r);
    
    std::string output = "";
    if (m.size() == 2) 
    {
        output = m.str(1);
    }
    
    return output;
}

//parse config to necessary attributes
bool meme_handler::parse_config ()
{
    path_to_memes_ = "";
    
    std::lock_guard<std::mutex> lock(config_lock_);
    
    std::string output;
    for (const auto& statement : config_->statements_) 
    {
        //statement we're looking for has exactly 2 tokens
         if (statement->tokens_.size() == 2)
         {
             if (statement->tokens_[0] == "memePath")
             {
                 path_to_memes_ = statement->tokens_[1];
             }
         }
    }
    
    return !(path_to_memes_ == "");
}

//check the provided body for valid input data
//TODO: implement this function
//What will the data look like?? Check out Form Post documentation
bool meme_handler::body_check(std::string body, std::vector<std::string>& params)
{
    //parse out necessary body parameters from request - into params
    //check memeSelect to ensure it matches the name of one of the template memes
    //check topCaption for a valid caption string
    //check bottomCaption for a valid caption string
    //return true if all checks are good, false otherwise
    return true;
}

//use the provided (valid) parameters to generate a new meme
//TODO: implement this function
//Idea: Create a file for each meme - file name is ID, contains meme template + captions
//This function creates a new meme file in the appropriate location
bool meme_handler::generate_new_meme (std::vector<std::string> params, std::string& new_id)
{
    //generate a new GUID to be the ID for the new meme - push its value to new_id
    //return true if no exception was encountered, false otherwise
    return true;
}