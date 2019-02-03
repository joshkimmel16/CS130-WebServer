#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "static_file_handler.h"

class StaticFileHandlerTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
    request* req;
    response* res;
    NginxConfigParser parser;
    NginxConfig out_config;
    
    virtual void SetUp() {
      parser.Parse("static/static_config", &out_config);
      config = out_config.statements_[1]->child_block_;
    }
};

//simple valid HTTP request for a file
//TODO: update this with a valid test once one can happen
TEST_F(StaticFileHandlerTest, SimpleGet) {
  std::string input = "GET /test/sample.jpg HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  req = new request(r, r_size);
    
  static_file_handler* out = new static_file_handler(config);
  res = out->handle_request(req);
  
  EXPECT_EQ(res->get_status_code(), 200); //status code should be 200
  //TODO: check Content-Type and body
    
  delete res;
  delete out;
  delete req;
}

//invalid method test
TEST_F(StaticFileHandlerTest, InvalidMethod) {
  std::string input = "DELETE /test HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  req = new request(r, r_size);
    
  static_file_handler* out = new static_file_handler(config);
  res = out->handle_request(req);
  
  EXPECT_EQ(res->get_body(), "This route only supports the HTTP GET method!"); //body should be the error message
  EXPECT_EQ(res->get_status_code(), 405); //status code should be 405
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain"); //Content-Type: text/plain header should be set
    
  delete res;
  delete out;
  delete req;
}