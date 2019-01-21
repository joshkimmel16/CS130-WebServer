//An HTTP Response

#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include "response.h"

//overriden constructor takes a status code and body
response::response (unsigned int status_code, std::string body)
{
    status_code_ = status_code;
    response_body_ = body;
}

//get status code
unsigned int response::get_status_code ()
{
    return status_code_;
}

//get a specific header by name
std::string response::get_header (std::string name)
{
    std::unordered_map<std::string, std::string>::const_iterator found = response_headers_.find(name);
    if (!(found == response_headers_.end()))
    {
        return found->second;
    }
    else
    {
        return "";
    }
}

//get response body
std::string response::get_body ()
{
    return response_body_;
}

//set the status code
void response::set_status_code (unsigned int sc)
{
    status_code_ = sc;
}

//set a specific header
void response::set_header (std::string h_name, std::string h_value)
{
    std::pair<std::string, std::string> h (h_name, h_value);
    response_headers_.insert(h);
}

//set the response body
void response::set_body (std::string body)
{
    response_body_ = body;
}

//generate a well-formed HTTP response based on internal attributes
//return the response as a C string so it can be written on a socket
const char* response::generate_response ()
{
    std::string output = "";
    output += generate_status_line();
    output += generate_header_lines();
    output += response_body_;
   
    response_ = output;
    return response_.c_str();
}

//generate a well-formed HTTP response status line based on internal attributes
std::string response::generate_status_line ()
{
    return "HTTP/1.1 " + std::to_string(status_code_) + " " + get_status_word(status_code_) + "\r\n";
}

//generate a well-formed HTTP response header line(s) based on internal attributes
std::string response::generate_header_lines ()
{
    std::string output = "";
    for (std::pair<std::string, std::string> header : response_headers_)
    {
	   output += (header.first + ": " + header.second + "\r\n");
    }
    return (output + "\r\n");
}

//return the status description corresponding to a given HTTP status
std::string response::get_status_word (unsigned int status)
{
    switch (status)
    {
        case (200):
            return "OK";
        case (400):
            return "Bad Request";
        default:
            return "";
    }
    return "";
}