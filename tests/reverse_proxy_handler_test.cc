#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "gtest/gtest.h"
#include "reverse_proxy_handler.h"

class ReverseProxyHandlerTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
    NginxConfigParser parser;
    NginxConfig out_config;

    void SetUp()
    {
      parser.Parse("proxy/proxy_config", &out_config);
      config = out_config.statements_[1]->child_block_->statements_[0]->child_block_;
    }
};

//simple valid HTTP request
TEST_F(ReverseProxyHandlerTest, SimpleGet) {
  std::string input = "GET /ucla HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));

  std::shared_ptr<route_handler> out = Reverse_proxy_handler::create_handler(config, "");

  std::shared_ptr<response> res = out->handle_request(req);
  EXPECT_EQ(res->get_status_code(), 200);
  EXPECT_STREQ(res->get_header("Content-Type").c_str(), std::string("text/html").c_str());
  EXPECT_STREQ(res->get_header("Content-Length").c_str(), std::to_string(res->get_body().length()).c_str());
}

TEST_F(ReverseProxyHandlerTest, GetSubDirectory) {
  std::string input = "GET /ucla/about HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));

  std::shared_ptr<route_handler> out = Reverse_proxy_handler::create_handler(config, "");

  std::shared_ptr<response> res = out->handle_request(req);
  EXPECT_EQ(res->get_status_code(), 200);
  EXPECT_STREQ(res->get_header("Content-Type").c_str(), std::string("text/html").c_str());
  EXPECT_STREQ(res->get_header("Content-Length").c_str(), std::to_string(res->get_body().length()).c_str());
}

TEST_F(ReverseProxyHandlerTest, GetRedirection) {
  std::string input = "GET /wiki HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));

  std::shared_ptr<route_handler> out = Reverse_proxy_handler::create_handler(config, "");

  std::shared_ptr<response> res = out->handle_request(req);
  EXPECT_EQ(res->get_status_code(), 200);
  EXPECT_STREQ(res->get_header("Content-Type").c_str(), std::string("text/plain").c_str());
  EXPECT_STREQ(res->get_header("Content-Length").c_str(), std::to_string(res->get_body().length()).c_str());
}