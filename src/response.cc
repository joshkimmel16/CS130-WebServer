
#include <iostream>
#include <string>

#include "resonse.h"


Response::Response() {}

std::string Response::get_ok_reponse(){
	  return ok_response;
}

std::string Response::get_request_msg(){
	  return request_msg;
}

void Response::set_request_msg(std::string request){
	  std::memcpy(request_msg, request, sizeof(request));
}
char* Response::to_string() const{
	  std::string out_msg = get_ok_response();
	    out_msg += hdr_content_type.first;
	      out_msg += ":";
	        out_msg += hdr_content_type.second;
		  out_msg += "\r\n";
		    out_msg += get_request_msg();

		      return out_msg.c_str();
}

