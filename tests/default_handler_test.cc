#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "default_handler.h"

class DefaultHandlerTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
    request* req;
    response* res;
};

//ensure response is as expected
TEST_F(DefaultHandlerTest, DefaultTest) {
  std::string input = "GET /test HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  req = new request(r, r_size);
    
  default_handler* out = new default_handler(config);
  res = out->handle_request(req);
  
  EXPECT_EQ(res->get_body(), "The requested resource at " + req->get_uri() + " could not be found!"); //body should be default message
  EXPECT_EQ(res->get_status_code(), 404); //status code should be 404
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain"); //Content-Type: text/plain header should be set
    
  delete res;
  delete out;
  delete req;
}