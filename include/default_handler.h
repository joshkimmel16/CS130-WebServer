//A handler for HTTP requests that don't match with any other handlers
#ifndef DEFAULT_HANDLER_H
#define DEFAULT_HANDLER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "route_handler.h"

class default_handler : public route_handler
{
public:
    //Methods
    default_handler (std::shared_ptr<NginxConfig> config); //constructor overload
    response* handle_request (request* req); //given a request, generate an appropriate response
private:    
    //Methods
    response* generate_404_response (request* req); //create a 404 response
};

#endif

