//A handler for HTTP requests that echoes the body of the request in the response
#ifndef ECHO_HANDLER_H
#define ECHO_HANDLER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "route_handler.h"

class echo_handler : public route_handler
{
public:
    //Methods
    echo_handler (std::shared_ptr<NginxConfig> config); //constructor overload
    response* handle_request (request* req); //given a request, generate an appropriate response
private:    
    //Methods
    response* generate_echo_response (request* req); //create an echo response for the given request
};

#endif

