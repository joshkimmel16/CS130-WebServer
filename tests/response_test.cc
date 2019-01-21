#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "response.h"

class ResponseTest : public ::testing::Test {
  protected:
    int status_code;
    std::string body;
};

// HTTP response to empty HTTP request
TEST_F(ResponseTest, ResponseToNoBody) {
  status_code = 400;
  body = "";
  response* resp = new response(status_code, body);

  std::string output = "HTTP/1.1 400 Bad Request\r\n\r\n";
  EXPECT_EQ(resp->get_status_code(), status_code);
  EXPECT_EQ(resp->get_body(), body);
  EXPECT_EQ(std::string(resp->generate_response()), output);
}

// HTTP response to request with body
TEST_F(ResponseTest, ResponseWithBody) {
  status_code = 200;
  body = "GET /test HTTP/1.1\r\n\r\n";
  response* resp = new response(status_code, body);

  std::string output = "HTTP/1.1 200 OK\r\n\r\nGET /test HTTP/1.1\r\n\r\n";
  EXPECT_EQ(resp->get_status_code(), status_code);
  EXPECT_EQ(resp->get_body(), body);
  EXPECT_EQ(std::string(resp->generate_response()), output);
}

// HTTP response to request with valid body and header
TEST_F(ResponseTest, ResponseWithBodyHeader) {
  status_code = 200;
  body = "GET /test HTTP/1.1\r\n\r\n";
  response* resp = new response(status_code, body);
  resp->set_header("Content-Type", "text/plain");

  std::string output = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nGET /test HTTP/1.1\r\n\r\n";
  EXPECT_EQ(resp->get_status_code(), status_code);
  EXPECT_EQ(resp->get_body(), body);
  EXPECT_EQ(resp->get_header("Content-Type"), "text/plain");
  EXPECT_EQ(std::string(resp->generate_response()), output);
}

// HTTP response to request with bad nonempty body
TEST_F(ResponseTest, ResponseWithBadBody) {
  status_code = 200;
  body = "hi\r\n\r\n";
  response* resp = new response(status_code, body);

  std::string output = "HTTP/1.1 200 OK\r\n\r\nhi\r\n\r\n";
  EXPECT_EQ(resp->get_status_code(), status_code);
  EXPECT_EQ(resp->get_body(), body);
  EXPECT_EQ(std::string(resp->generate_response()), output);
}

