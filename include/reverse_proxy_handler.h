#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "route_handler.h"

class Reverse_proxy_handler : public route_handler
{
public:
    static std::shared_ptr<route_handler> create_handler (std::shared_ptr<NginxConfig> config, std::string root_path);
    std::shared_ptr<response> handle_request (std::shared_ptr<request> req); //given a request, generate an appropriate response
    // static std::unordered_map<std::string, std::string> get_proxies (std::shared_ptr<NginxConfig> config);
private:
    Reverse_proxy_handler (std::shared_ptr<NginxConfig> config, std::string root_path); //constructor overload
    // static std::unordered_map<std::string, std::string> location2proxy;
};