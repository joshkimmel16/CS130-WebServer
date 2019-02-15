//A handler for that shows the status of the web server 

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
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
    return generate_status_response(req);
}

//the provided URI did not match any defined handler
//so send a 404 response with an appropriate message body
std::shared_ptr<response> status_handler::generate_status_response (std::shared_ptr<request> req)
{
    std::shared_ptr<response> resp(new response(200, ("Status message goes here.")));
    resp->set_header("Content-Type", "text/plain");
    return resp;
}