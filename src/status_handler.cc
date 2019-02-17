//A handler for that shows the status of the web server 

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <sstream>
#include "status_handler.h"

//overriden constructor takes a config
//for this route, no config is used so the input is expected to be NULL
status_handler::status_handler (std::shared_ptr<NginxConfig> config, std::string root_path) : route_handler(config, root_path) {}

//overriden factory method to create a new instance
std::shared_ptr<route_handler> status_handler::create_handler (std::shared_ptr<NginxConfig> config, std::string root_path)
{
   
    return std::shared_ptr<status_handler>(new status_handler(config, root_path));
}

//overridden method in parent class to handle a request
std::shared_ptr<response> status_handler::handle_request (std::shared_ptr<request> req)
{
    int num_request = server_status_recorder::get_instance().get_url_response_list_size();
    if(num_request < 0)
    {
        //LOG_FATAL << "Something is not right with request status handler";
        exit(1);
    }

    std::vector<std::pair<std::string, unsigned int>> url_response_list = server_status_recorder::get_instance().get_url_response_list();
    std::ostringstream body;
    body << "<!DOCTYPE html><html><head></head><body><h1>Server Status</h1><p>Number of request: "
         << num_request
	 << "</p>"
         << "<table>"
         << "<tr><th>URL</th><th>Response Code</th></tr>";
    if(num_request == 0)
    {
         body << "</tr><td colspan='2'>No Request</td><tr>";
    }
    else
    {
         for (auto request : url_response_list)
         {
              body << "<tr><td>" << request.first  << "</td>"
                   << "<td>"     << request.second << "</td></tr>";
         }
    }
    body << "</table>";
    
    std::vector<std::pair<std::string, std::string>> prefix_handler_list = server_status_recorder::get_instance().get_prefix_handler_list();
    body << "<p>URL Prefix and Handler Matching</p>"
	 << "<table>"
	 << "<tr><th>Prefix</th><th>Handler</th></tr>";
    if(prefix_handler_list.empty())
    {
	 body << "</tr><td colspan= '2'>Empty</td><tr>";
    }
    else
    {
	 for (auto pre : prefix_handler_list)
	 {
              body << "<tr><td>" << pre.first  << "</td>"
		   << "<td>"     << pre.second << "</td></tr>";
	 }
    }
    body << "</table></body></html>";

    return generate_status_response(req, body.str());
}

//the provided URI did not match any defined handler
//so send a 404 response with an appropriate message body
std::shared_ptr<response> status_handler::generate_status_response(std::shared_ptr<request> req, std::string body)
{
    std::shared_ptr<response> resp(new response(200, ("Status message goes here.")));
    resp->set_header("Content-Type", "text/html");
    resp->set_body(body);
    return resp;
}
