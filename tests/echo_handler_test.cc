#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "echo_handler.h"

class EchoHandlerTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
    request* req;
    response* res;
};

//simple valid HTTP request
TEST_F(EchoHandlerTest, SimpleGet) {
  std::string input = "GET /test HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  req = new request(r, r_size);
    
  echo_handler* out = new echo_handler(config);
  res = out->handle_request(req);
  
  EXPECT_EQ(res->get_body(), "GET /test HTTP/1.1\r\n\r\n"); //body should be equal to the request
  EXPECT_EQ(res->get_status_code(), 200); //status code should be 200
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain"); //Content-Type: text/plain header should be set
    
  delete res;
  delete out;
  delete req;
}