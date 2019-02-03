//A handler for HTTP requests that serves static files

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "static_file_handler.h"

//overriden constructor takes a config
//for this route, a config is required that contains a path to the static images
static_file_handler::static_file_handler (std::shared_ptr<NginxConfig> config) : route_handler(config) 
{
    get_static_file_path();
}

//overridden method in parent class to handle a request
response* static_file_handler::handle_request (request* req)
{
    std::string method = req->get_method();
    if (method == "GET")
    {
        return serve_file(req);
    }
    else
    {
        return invalid_method(req);
    }
}

//parse config to retrieve the static file path
bool static_file_handler::get_static_file_path ()
{
    std::string output;
    for (const auto& statement : config_->statements_) 
    {
        //statement we're looking for has exactly 2 tokens
        if (statement->tokens_.size() == 2)
        {
            if (statement->tokens_[0] == "path")
            {
                output = statement->tokens_[1];
            }
        }
    }
    if (!output.empty())
    {
        path_ = output;
        return true;
    }
    else
    {
        return false;
    }
}

//serve the file indicated by the URI
//if the file isn't present, return 404
response* static_file_handler::serve_file (request* req)
{
    return new response(200, "You got served!");
    //TODO: actually implement this
}

//the method used in the request wasn't valid so return a 405
response* static_file_handler::invalid_method(request* req)
{
    response* resp = new response(405, "This route only supports the HTTP GET method!");
    resp->set_header("Content-Type", "text/plain");
    return resp;
}