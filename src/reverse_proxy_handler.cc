#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "reverse_proxy_handler.h"

Reverse_proxy_handler::Reverse_proxy_handler(std::shared_ptr<NginxConfig> config, std::string root_path) : route_handler(config, root_path) 
{
    std::cout << config->ToString() << std::endl;
}

std::shared_ptr<route_handler> Reverse_proxy_handler::create_handler (std::shared_ptr<NginxConfig> config, std::string root_path)
{
    return std::shared_ptr<Reverse_proxy_handler>(new Reverse_proxy_handler(config, root_path));
}

std::shared_ptr<response> Reverse_proxy_handler::handle_request (std::shared_ptr<request> req)
{
    // TODO: Chengyao
    // send request to actual proxy and generate response
    std::shared_ptr<response> resp(new response(200, std::string(req->get_raw_request())));
    resp->set_header("Content-Type", "text/plain");
    return resp;
}

// std::unordered_map<std::string, std::string> Reverse_proxy_handler::get_proxies (std::shared_ptr<NginxConfig> config)
// {
//     return location2proxy;
// }