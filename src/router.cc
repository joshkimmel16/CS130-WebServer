//A router to delegate HTTP requests to appropriate handlers

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "router.h"
#include "echo_handler.h"
#include "default_handler.h"

//constructor takes a config
router::router (std::shared_ptr<NginxConfig> config)
{
    config_ = config;
}

//register a handler to a particular URI
//inputs are a URI and handler name as strings
//handler names should correspond to defined handlers
bool router::register_route (std::string uri, std::string handler_name)
{
    auto it = route_map_.find(uri);
    if (it != route_map_.end())
    {
	    it->second = handler_name;
    }
    else
    {
        std::pair<std::string, std::string> h (uri, handler_name);
        route_map_.insert(h);
    }
}

//capture a default HTTP header to apply to all responses from the server
//inputs are strings for the header name and value
bool router::register_default_header (std::string header_name, std::string header_value)
{
    auto it = headers_.find(header_name);
    if (it != headers_.end())
    {
	    it->second = header_value;
    }
    else
    {
        std::pair<std::string, std::string> h (header_name, header_value);
        headers_.insert(h);
    }
}

//main method to route the request to a particular handler and return the generated response
//input is a valid HTTP request
response* router::route_request (request* req)
{
    route_handler* rh = select_handler(req->get_uri());
    response* output = rh->handle_request(req);
    apply_default_headers(output);
    delete rh;
    return output;
}

//get a specific route handler by URI
std::string router::get_route_handler (std::string uri)
{
    std::unordered_map<std::string, std::string>::const_iterator found = route_map_.find(uri);
    if (!(found == route_map_.end()))
    {
        return found->second;
    }
    else
    {
        return "";
    }
}

//get a specific header by name
std::string router::get_header (std::string name)
{
    std::unordered_map<std::string, std::string>::const_iterator found = headers_.find(name);
    if (!(found == headers_.end()))
    {
        return found->second;
    }
    else
    {
        return "";
    }
}

//get a nested config from the master config for a specific route handler
//TODO: figure out how to pass the config
std::shared_ptr<NginxConfig> router::get_handler_config (std::string handler_name)
{
    std::shared_ptr<NginxConfig> output;
    for (const auto& statement : config_->statements_) 
    {
        //the config we're looking for is nested
        if (statement->child_block_)
        {
            //must have at least one element if the child block is not null
            if (statement->tokens_[0] == handler_name)
            {
                output = statement->child_block_;
            }
        }
    }
    return output;
}

//apply the default headers for the server to the given response
bool router::apply_default_headers (response* res)
{
    for (std::pair<std::string, std::string> header : headers_)
    {
	   res->set_header(header.first, header.second);
    }
    return true;
}

//return the appropriate route_handler given the provided URI 
//TODO: add handler for static files once implemented
//TODO: this doesn't handle query strings in the URI well
route_handler* router::select_handler (std::string uri)
{
    if (uri == "/echo")
    {
        return new echo_handler(get_handler_config("echo"));
    }
    else
    {
        return new default_handler(get_handler_config("default"));
    }
}