
#include "server_status_recorder.h"

server_status_recorder& server_status_recorder::get_instance()
{
    static server_status_recorder instance_;
    return instance_;
}

void server_status_recorder::request_recorder(const std::string& url, const unsigned int response_code)
{
    std::pair<std::string, unsigned int> url_response(url, response_code);
    url_response_list_.push_back(url_response);
}

std::vector<std::pair<std::string, unsigned int>> server_status_recorder::get_url_response_list() const
{
    return url_response_list_;
}

int server_status_recorder::get_url_response_list_size()
{
    return url_response_list_.size();
}


