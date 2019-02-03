#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "router.h"

class RouterTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
    request* req;
    response* res;
    NginxConfigParser parser;
    NginxConfig out_config;
    
    virtual void SetUp() {
      parser.Parse("router/router_config", &out_config);
      config = out_config.statements_[1]->child_block_;
    }
};

//ensure routes are properly registered
TEST_F(RouterTest, RegisterRoute) {
  router* out = new router(config);
  out->register_route("/echo", "echo");
  
  EXPECT_EQ(out->get_route_handler("/echo"), "echo"); //route should be registered appropriately
  EXPECT_EQ(out->get_route_handler("/random"), ""); //missing route should return empty string
    
  delete out;
}

//ensure headers are properly registered
TEST_F(RouterTest, RegisterHeader) {
  router* out = new router(config);
  out->register_default_header("Content-Type", "application/json");
  
  EXPECT_EQ(out->get_header("Content-Type"), "application/json"); //header should be registered appropriately
  EXPECT_EQ(out->get_header("User-Agent"), ""); //missing header should return empty string
    
  delete out;
}

//test echo route
TEST_F(RouterTest, Echo) {
  router* out = new router(config);
  out->register_route("/echo", "echo");
  out->register_default_header("User-Agent", "testing");
  
  std::string input = "GET /echo HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  req = new request(r, r_size);
  
  res = out->route_request(req);
  EXPECT_EQ(res->get_status_code(), 200); //should have a 200 status code
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain"); //route header should be registered appropriately
  EXPECT_EQ(res->get_header("User-Agent"), "testing"); //default header should be registered appropriately
  EXPECT_EQ(res->get_body(), "GET /echo HTTP/1.1\r\n\r\n"); //body should be the request
    
  delete out;
  delete req;
  delete res;
}

//test static route
TEST_F(RouterTest, Static) {
  router* out = new router(config);
  out->register_route("/static1/.*", "static1");
  out->register_default_header("User-Agent", "testing");
  
  std::string input = "GET /static1/image.jpg HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  req = new request(r, r_size);
  
  res = out->route_request(req);
  EXPECT_EQ(res->get_status_code(), 200); //should have a 200 status code
  EXPECT_EQ(res->get_header("User-Agent"), "testing"); //default header should be registered appropriately
    
  delete out;
  delete req;
  delete res;
}

//test default handling
TEST_F(RouterTest, Default) {
  router* out = new router(config);
  out->register_route("/echo", "echo");
  out->register_default_header("User-Agent", "testing");
  
  std::string input = "GET /random HTTP/1.1\r\n\r\n";
  int n = input.length();    
  char char_array[n+1];   
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  req = new request(r, r_size);
  
  res = out->route_request(req);
  EXPECT_EQ(res->get_status_code(), 404); //should have a 404 status code
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain"); //route header should be registered appropriately
  EXPECT_EQ(res->get_header("User-Agent"), "testing"); //default header should be registered appropriately
  EXPECT_EQ(res->get_body(), "The requested resource at " + req->get_uri() + " could not be found!"); //body should be default message
    
  delete out;
  delete req;
  delete res;
}