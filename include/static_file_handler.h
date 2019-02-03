//A handler for HTTP requests that serves static files
#ifndef STATIC_FILE_HANDLER_H
#define STATIC_FILE_HANDLER_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "route_handler.h"

class static_file_handler : public route_handler
{
public:
    //Methods
    static_file_handler (std::shared_ptr<NginxConfig> config); //constructor overload
    response* handle_request (request* req); //given a request, generate an appropriate response
private:    
    //Methods
    bool get_static_file_path (); //retrieve the file path for static files from the config
    response* serve_file (request* req); //serve static file
    response* invalid_method (request* req); //generate invalid method response
    
    //Attributes
    std::string path_;
};

#endif

