//where it records request uri and response.
#ifndef SERVER_STATUS_RECORDER_H
#define SERVER_STATUS_RECORDER_H

#include <string>
#include <vector>


class server_status_recorder
{
public:
    //singleton instance
    static server_status_recorder& get_instance();
    //get request url and response code
    void request_recorder(const std::string& url, const unsigned int response_code);
    //return list of request url and response code pair
    std::vector<std::pair<std::string, unsigned int>> get_url_response_list() const;
    //return size of url and code list
    int get_url_response_list_size();


private:
    //private constructor since singleton 
    server_status_recorder() {};
    //list of request url and response code
    std::vector<std::pair<std::string, unsigned int>> url_response_list_;
};

#endif
