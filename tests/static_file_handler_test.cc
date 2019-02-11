#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>
#include "gtest/gtest.h"
#include "static_file_handler.h"
#include <unistd.h>

class StaticFileHandlerTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
    NginxConfigParser parser;
    NginxConfig out_config;
    
    virtual void SetUp() {
      parser.Parse("static/static_config", &out_config);
      config = out_config.statements_[1]->child_block_->statements_[0]->child_block_;
    }
};

//simple valid HTTP request for a file
//TODO: update this with a valid test once one can happen
TEST_F(StaticFileHandlerTest, StaticTest) {
  std::string input = "GET /static1/text.txt HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));
    
  std::unique_ptr<static_file_handler> out(new static_file_handler(config));
  std::vector<std::string> check = out->parse_file_info(req->get_uri());
  std::string test = (out->get_static_file_path() + "/" + check[0] + check[1]);
  EXPECT_EQ(test, "static/static1/text.txt");
    
  std::shared_ptr<response> res = out->handle_request(req);
  EXPECT_EQ(res->get_status_code(), 200);
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain");
  EXPECT_EQ(res->get_body(), "abc");
}

//invalid method test
TEST_F(StaticFileHandlerTest, InvalidMethod) {
  std::string input = "DELETE /test HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));
    
  std::unique_ptr<static_file_handler> out(new static_file_handler(config));
  std::shared_ptr<response> res = out->handle_request(req);
  
  EXPECT_EQ(res->get_body(), "This route only supports the HTTP GET method!"); //body should be the error message
  EXPECT_EQ(res->get_status_code(), 405); //status code should be 405
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain"); //Content-Type: text/plain header should be set
}