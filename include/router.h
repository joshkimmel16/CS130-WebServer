//A router to delegate HTTP requests to appropriate handlers
#ifndef ROUTER_H
#define ROUTER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "route_handler.h"

class router
{
public:
    //Methods
    router (std::shared_ptr<NginxConfig> config); //constructor overload
    bool register_route (std::string uri, std::string handler_name); //register handler for the provided URI
    bool register_default_header (std::string h_name, std::string h_value); //bind a default header to all responses from the server
    response* route_request (request* req); //apply the appropriate handler to the provided request and generate a response
    std::string get_route_handler (std::string uri); //retrieve the handler for the provided route
    std::string get_header(std::string name); //retrieve a given header by name
private:
    //Methods
    std::shared_ptr<NginxConfig> get_handler_config (std::string handler_name); //retrieve the config for a particular handler
    bool apply_default_headers (response* res); //apply default headers to a given response
    route_handler* select_handler (std::string uri); //retrieve the appropriate handler object given a URI
    
    //Attributes
    std::shared_ptr<NginxConfig> config_;
    std::unordered_map<std::string, std::string> route_map_;
    std::unordered_map<std::string, std::string> headers_;
};

#endif

