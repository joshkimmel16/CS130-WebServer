#include <iostream>
#include <string>
#include "gtest/gtest.h"
#include "health_handler.h"

class HealthHandlerTest : public ::testing::Test {
  protected:
    char* r;
    size_t r_size;
    std::shared_ptr<NginxConfig> config;
};

TEST_F(HealthHandlerTest, HealthTest) {
  std::string input = "GET /health HTTP/1.1\r\n\r\n";
  int n = input.length();
  char char_array[n+1];
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));
 
  std::shared_ptr<route_handler> out = health_handler::create_handler(config, "HOLDER");
  std::shared_ptr<response> res = out->handle_request(req);

  EXPECT_EQ(res->get_status_code(), 200);
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain");
  EXPECT_EQ(res->get_body(), "OK");
}

TEST_F(HealthHandlerTest, HealthBadSubdirTest) {
  std::string input = "GET /health/valid HTTP/1.1\r\n\r\n";
  int n = input.length();
  char char_array[n+1];
  strcpy(char_array, input.c_str());
  r = char_array;
  r_size = std::strlen(char_array);
  std::shared_ptr<request> req(new request(r, r_size));
 
  std::shared_ptr<route_handler> out = health_handler::create_handler(config, "HOLDER");
  std::shared_ptr<response> res = out->handle_request(req);

  EXPECT_NE(res->get_status_code(), 200);
//:  EXPECT_EQ(res->get_header("Content-Type"), "text/plain");
}
