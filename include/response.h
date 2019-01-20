#include <string>
#include <utility>


class Response{
	public:
		  Response();

		    std::string get_ok_response();
		      std::string get_request_msg();

		        void set_request_msg(std::string request);

			  char* to_string() const;

	private:
			    std::pair <std::string, std::string> hdr_content_type {"Content-Type", "text/plain"};
			      const std::string ok_response =  "HTTP/1.1 200 OK\r\n";
			        char *request_msg[1024];
};

