//An interface to be implemented by all route handlers designed to handle specific requests
#ifndef ROUTE_HANDLER_H
#define ROUTE_HANDLER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "config_parser.h"
#include "request.h"
#include "response.h"

class route_handler
{
public:
    //Methods
    route_handler (std::shared_ptr<NginxConfig> config) { config_ = config; }; //constructor overload
    virtual std::shared_ptr<response> handle_request (std::shared_ptr<request> req)=0; //given a request, generate an appropriate response
protected:    
    //Attributes
    std::shared_ptr<NginxConfig> config_;
};

#endif

