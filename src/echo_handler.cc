//A handler for HTTP requests that echoes the body of the request in the response

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "echo_handler.h"

//overriden constructor takes a config
//for this route, no config is used so the input is expected to be NULL
echo_handler::echo_handler (std::shared_ptr<NginxConfig> config) : route_handler(config) {}

//overridden method in parent class to handle a request
response* echo_handler::handle_request (request* req)
{
    return generate_echo_response(req);
}

//echo the HTTP request received in the body of the response
//since validation occurs earlier, always respond with a 200
response* echo_handler::generate_echo_response (request* req)
{
    response* resp = new response(200, std::string(req->get_raw_request()));
    resp->set_header("Content-Type", "text/plain");
    return resp;
}