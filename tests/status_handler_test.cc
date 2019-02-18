#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "status_handler.h"

class StatusHandlerTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
};

//ensure response is as expected
TEST_F(StatusHandlerTest, DefaultTest) {
  std::string input = "GET /status HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));
    
  std::shared_ptr<route_handler> out = status_handler::create_handler(config, "HOLDER");
  std::shared_ptr<response> res = out->handle_request(req);

  
  EXPECT_EQ(res->get_body(), 
  "<!DOCTYPE html><html><head></head><body><h1>Server Status</h1><p>Number of request: 0</p><table><tr><th>URL</th><th>Response Code</th></tr></tr><td colspan='2'>No Request</td><tr></table><p>URL Prefix and Handler Matching</p><table><tr><th>Prefix</th><th>Handler</th></tr></tr><td colspan= '2'>Empty</td><tr></table></body></html>"
  ); //body should be status page with no requests yet
  EXPECT_EQ(res->get_status_code(), 200); //status code should be 200
  EXPECT_EQ(res->get_header("Content-Type"), "text/html"); //Content-Type: text/html should be set
}

TEST_F(StatusHandlerTest, OneRouteHandlerTest) {
  std::string input = "GET /status HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));
    
  std::shared_ptr<route_handler> out = status_handler::create_handler(config, "HOLDER");
  std::pair<std::string, std::string> h ("/echo", "echo");
  server_status_recorder::get_instance().prefix_recorder(h);

  std::shared_ptr<response> res = out->handle_request(req);

  
  EXPECT_EQ(res->get_body(), 
  "<!DOCTYPE html><html><head></head><body><h1>Server Status</h1><p>Number of request: 0</p><table><tr><th>URL</th><th>Response Code</th></tr></tr><td colspan='2'>No Request</td><tr></table><p>URL Prefix and Handler Matching</p><table><tr><th>Prefix</th><th>Handler</th></tr><tr><td>/echo</td><td>echo</td></tr></table></body></html>"
  ); //body should be status page with echo handler
  EXPECT_EQ(res->get_status_code(), 200); //status code should be 200
  EXPECT_EQ(res->get_header("Content-Type"), "text/html"); //Content-Type: text/html should be set
}

TEST_F(StatusHandlerTest, OneRequestHandlerTest) {
  std::string input = "GET /status HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));
    
  std::shared_ptr<route_handler> out = status_handler::create_handler(config, "HOLDER");
  server_status_recorder::get_instance().request_recorder("/static1/myimage.jpg", 200);

  std::shared_ptr<response> res = out->handle_request(req);

  
  EXPECT_EQ(res->get_body(), 
  "<!DOCTYPE html><html><head></head><body><h1>Server Status</h1><p>Number of request: 1</p><table><tr><th>URL</th><th>Response Code</th></tr><tr><td>/static1/myimage.jpg</td><td>200</td></tr></table><p>URL Prefix and Handler Matching</p><table><tr><th>Prefix</th><th>Handler</th></tr></tr><td colspan= '2'>Empty</td><tr></table></body></html>"
  ); //body should be status page with one request
  EXPECT_EQ(res->get_status_code(), 200); //status code should be 200
  EXPECT_EQ(res->get_header("Content-Type"), "text/html"); //Content-Type: text/html should be set
}
