//A handler for HTTP requests that don't match with any other handlers

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "default_handler.h"

//overriden constructor takes a config
//for this route, no config is used so the input is expected to be NULL
default_handler::default_handler (std::shared_ptr<NginxConfig> config) : route_handler(config) {}

//overridden method in parent class to handle a request
response* default_handler::handle_request (request* req)
{
    return generate_404_response(req);
}

//the provided URI did not match any defined handler
//so send a 404 response with an appropriate message body
response* default_handler::generate_404_response (request* req)
{
    response* resp = new response(404, ("The requested resource at " + req->get_uri() + " could not be found!"));
    resp->set_header("Content-Type", "text/plain");
    return resp;
}