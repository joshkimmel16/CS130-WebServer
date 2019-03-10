#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "meme_handler.h"
#include <unistd.h>
#include <vector>

using ::testing::HasSubstr;

class MemeHandlerTest : public ::testing::Test {
  protected:
    std::shared_ptr<NginxConfig> config;
    NginxConfigParser parser;
    NginxConfig out_config;
    char* req_char_arr;
    size_t req_len;
    virtual void SetUp() {
      parser.Parse("memes/meme_config", &out_config);
      config = out_config.statements_[0]->child_block_->statements_[0]->child_block_;
    }
};

//add 3 new memes and verify meme list returns them correctly
TEST_F(MemeHandlerTest, MemeListTest) {
  std::string input = "POST /memes HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nCache-Control: max-age=0\r\nContent-Length: 54\r\n\r\nmemeSelect=grumpy.jpg&topCaption=123&bottomCaption=123";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  std::string firstID = res->get_body().substr(7, 38);
  
  EXPECT_EQ(res->get_status_code(), 201);
  EXPECT_EQ(res->get_header("Content-Type"), "application/json");

  std::string input2 = "POST /memes HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nCache-Control: max-age=0\r\nContent-Length: 54\r\n\r\nmemeSelect=simply.jpg&topCaption=123&bottomCaption=123";
  const int n2 = input2.length();
  char char_arr2[n2+1];
  strcpy(char_arr2, input2.c_str());
  req_char_arr = char_arr2;
  req_len = std::strlen(char_arr2);
  std::shared_ptr<request> req2(new request(req_char_arr, req_len));

  std::shared_ptr<response> res2 = out->handle_request(req2);

  std::string secondID = res2->get_body().substr(7, 38);
  
  EXPECT_EQ(res2->get_status_code(), 201);
  EXPECT_EQ(res2->get_header("Content-Type"), "application/json");

  std::string input3 = "POST /memes HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nCache-Control: max-age=0\r\nContent-Length: 57\r\n\r\nmemeSelect=ermahgerd.jpg&topCaption=123&bottomCaption=123";
  const int n3 = input3.length();
  char char_arr3[n3+1];
  strcpy(char_arr3, input3.c_str());
  req_char_arr = char_arr3;
  req_len = std::strlen(char_arr3);
  std::shared_ptr<request> req3(new request(req_char_arr, req_len));

  std::shared_ptr<response> res3 = out->handle_request(req3);

  std::string thirdID = res3->get_body().substr(7, 38);
  
  EXPECT_EQ(res3->get_status_code(), 201);
  EXPECT_EQ(res3->get_header("Content-Type"), "application/json");

  std::string input4 = "GET /memes HTTP/1.1\r\n\r\n";
  const int n4 = input4.length();
  char char_arr4[n4+1];
  strcpy(char_arr4, input4.c_str());
  req_char_arr = char_arr4;
  req_len = std::strlen(char_arr4);
  std::shared_ptr<request> req4(new request(req_char_arr, req_len));

  std::shared_ptr<response> res4 = out->handle_request(req4);

  EXPECT_THAT(res4->get_body(), HasSubstr(firstID));
  EXPECT_THAT(res4->get_body(), HasSubstr(secondID));
  EXPECT_THAT(res4->get_body(), HasSubstr(thirdID));
}

//test that only post and get methods are valid
TEST_F(MemeHandlerTest, InvalidMethodTest) {
  std::string input = "TRACE /memes HTTP/1.1\r\n\r\n";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  EXPECT_EQ(res->get_status_code(), 405);
  EXPECT_EQ(res->get_body(), "This route only supports the HTTP GET and POST methods!");
}

TEST_F(MemeHandlerTest, ViewRedirectTest) {
  std::string input = "POST /memes HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nCache-Control: max-age=0\r\nContent-Length: 54\r\n\r\nmemeSelect=grumpy.jpg&topCaption=123&bottomCaption=123";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  std::string firstID = res->get_body().substr(8, 36);
  
  EXPECT_EQ(res->get_status_code(), 201);
  EXPECT_EQ(res->get_header("Content-Type"), "application/json");

  std::string input2 = "GET /memes/view/" + firstID + " HTTP/1.1\r\n\r\n";
  const int n2 = input.length();
  char char_arr2[n2+1];
  strcpy(char_arr2, input2.c_str());
  req_char_arr = char_arr2;
  req_len = std::strlen(char_arr2);
  std::shared_ptr<request> req2(new request(req_char_arr, req_len));

  std::shared_ptr<response> res2 = out->handle_request(req2);

  EXPECT_EQ(res2->get_status_code(), 303);
  EXPECT_EQ(res2->get_header("Location"), "/web/memes/view.html?id=" + firstID);
}

//Meme create good redirect test
TEST_F(MemeHandlerTest, CreateBasicTest) {
  std::string input = "GET /memes/create HTTP/1.1\r\n\r\n";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  EXPECT_EQ(res->get_status_code(), 303);
  EXPECT_EQ(res->get_header("Location"), "/web/memes/create.html");
}

//Meme list good redirect test
TEST_F(MemeHandlerTest, ListBasicTest) {
  std::string input = "GET /memes/view/list HTTP/1.1\r\n\r\n";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  EXPECT_EQ(res->get_status_code(), 303);
  EXPECT_EQ(res->get_header("Location"), "/web/memes/list.html");
}

//Meme create redirect bad test
TEST_F(MemeHandlerTest, BadCreateTest) {
  std::string input = "GET /memes/createblah HTTP/1.1\r\n\r\n";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  EXPECT_EQ(res->get_status_code(), 404);
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain");
  EXPECT_EQ(res->get_body(), "The requested file could not be found!\n");
}

//Meme list redirect bad test
TEST_F(MemeHandlerTest, BadListTest) {
  std::string input = "GET /memes/viewblah/listblah HTTP/1.1\r\n\r\n";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  EXPECT_EQ(res->get_status_code(), 404);
  EXPECT_EQ(res->get_header("Content-Type"), "text/plain");
  EXPECT_EQ(res->get_body(), "The requested file could not be found!\n");  
}

//Meme create good request: check sending selected option works with many arguments in header
TEST_F(MemeHandlerTest, CreateGoodLongMemeReqTest) {
  std::string input = "POST /memes HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nCache-Control: max-age=0\r\nContent-Length: 54\r\n\r\nmemeSelect=grumpy.jpg&topCaption=123&bottomCaption=123";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);
  
  EXPECT_EQ(res->get_status_code(), 201);
  EXPECT_EQ(res->get_header("Content-Type"), "application/json");
}

//Meme create good request: check sending selected option works
TEST_F(MemeHandlerTest, CreateGoodMemeReqTest) {
  std::string input = "POST /memes HTTP/1.1\r\n\r\nmemeSelect=grumpy.jpg&topCaption=123&bottomCaption=123";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);
  
  EXPECT_EQ(res->get_status_code(), 201);
  EXPECT_EQ(res->get_header("Content-Type"), "application/json");
}

//Meme create test with too little arguments
TEST_F(MemeHandlerTest, CreateBadMemeReqTest) {
  std::string input = "POST /memes HTTP/1.1\r\n\r\nmemeSelect=grumpy.jpg";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  EXPECT_EQ(res->get_status_code(), 400);
  EXPECT_EQ(res->get_body(), "The provided meme creation request is invalid!\n");
}

//Meme create good really long request sent to check sendingselected option works
TEST_F(MemeHandlerTest, CreateAndViewTest) {
  std::string input = "POST /memes HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nCache-Control: max-age=0\r\nContent-Length: 54\r\n\r\nmemeSelect=ermahgerd.jpg&topCaption=LLONG&bottomCaption=TOOOOO+LONG";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  std::string resp_body = res->get_body();
  std::string id = "";
  std::size_t id_loc = resp_body.find("id");
  if(id_loc != std::string::npos)
  {
    id = resp_body.substr(id_loc + 6, 36);
  }

  EXPECT_NE(id, "");

  std::string new_request = "GET /memes/";
  new_request += id;
  new_request += " HTTP/1.1\r\n\r\n";

  const int n2 = input.length();
  char char_arr2[n2 + 1];
  strcpy(char_arr2, new_request.c_str());
  req_char_arr = char_arr2;
  req_len = std::strlen(char_arr2);
  std::shared_ptr<request> req2(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out2 = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res2 = out->handle_request(req2);

  EXPECT_EQ(res2->get_status_code(), 200);
  EXPECT_EQ(res2->get_header("Content-Type"), "application/json");

  std::string body = res2->get_body();
  std::size_t pic_loc = body.find("memeSelect") + 14;
  std::size_t top_loc = body.find("topCaption") + 14;
  std::size_t bot_loc = body.find("bottomCaption") + 17;
  //memeSelect=ermahgerd.jpg&topCapt
  //ion=LLONG&bottomCaption=TOOOOO+LONG";
  EXPECT_EQ(body.substr(pic_loc, 13), "ermahgerd.jpg");
  EXPECT_EQ(body.substr(top_loc, 5), "LLONG");
  EXPECT_EQ(body.substr(bot_loc, 11), "TOOOOO LONG");
}

TEST_F(MemeHandlerTest, GoodUpdateTest) {
  std::string input = "POST /memes HTTP/1.1\r\n\r\nmemeSelect=grumpy.jpg&topCaption=123&bottomCaption=123";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);
  
  EXPECT_EQ(res->get_status_code(), 201);
    
  std::string resp_body = res->get_body();
  std::string id = "";
  std::size_t id_loc = resp_body.find("id");
  if(id_loc != std::string::npos)
  {
    id = resp_body.substr(id_loc + 6, 36);
  }

  EXPECT_NE(id, "");
    
  std::string new_request = "PUT /memes/";
  new_request += id;
  new_request += " HTTP/1.1\r\n\r\nmemeSelect=simply.jpg&topCaption=456&bottomCaption=789";
    
  const int n2 = input.length();
  char char_arr2[n2 + 1];
  strcpy(char_arr2, new_request.c_str());
  req_char_arr = char_arr2;
  req_len = std::strlen(char_arr2);
  std::shared_ptr<request> req2(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out2 = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res2 = out->handle_request(req2);

  EXPECT_EQ(res2->get_status_code(), 200);
}

TEST_F(MemeHandlerTest, BadUpdateTest) {
  std::string input = "POST /memes HTTP/1.1\r\n\r\nmemeSelect=grumpy.jpg&topCaption=123&bottomCaption=123";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);
  
  EXPECT_EQ(res->get_status_code(), 201);
    
  std::string resp_body = res->get_body();
  std::string id = "";
  std::size_t id_loc = resp_body.find("id");
  if(id_loc != std::string::npos)
  {
    id = resp_body.substr(id_loc + 6, 36);
  }

  EXPECT_NE(id, "");
    
  std::string new_request = "PUT /memes/";
  new_request += "absolutegarbage";
  new_request += " HTTP/1.1\r\n\r\nmemeSelect=simply.jpg&topCaption=456&bottomCaption=789";
    
  const int n2 = input.length();
  char char_arr2[n2 + 1];
  strcpy(char_arr2, new_request.c_str());
  req_char_arr = char_arr2;
  req_len = std::strlen(char_arr2);
  std::shared_ptr<request> req2(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out2 = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res2 = out->handle_request(req2);

  EXPECT_EQ(res2->get_status_code(), 400);
}

TEST_F(MemeHandlerTest, GoodDeleteTest) {
  std::string input = "POST /memes HTTP/1.1\r\n\r\nmemeSelect=grumpy.jpg&topCaption=123&bottomCaption=123";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);
  
  EXPECT_EQ(res->get_status_code(), 201);
    
  std::string resp_body = res->get_body();
  std::string id = "";
  std::size_t id_loc = resp_body.find("id");
  if(id_loc != std::string::npos)
  {
    id = resp_body.substr(id_loc + 6, 36);
  }

  EXPECT_NE(id, "");
    
  std::string new_request = "DELETE /memes/";
  new_request += id;
  new_request += " HTTP/1.1\r\n\r\n";
    
  const int n2 = input.length();
  char char_arr2[n2 + 1];
  strcpy(char_arr2, new_request.c_str());
  req_char_arr = char_arr2;
  req_len = std::strlen(char_arr2);
  std::shared_ptr<request> req2(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out2 = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res2 = out->handle_request(req2);

  EXPECT_EQ(res2->get_status_code(), 200);
}

TEST_F(MemeHandlerTest, BadDeleteTest) {
  std::string input = "POST /memes HTTP/1.1\r\n\r\nmemeSelect=grumpy.jpg&topCaption=123&bottomCaption=123";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);
  
  EXPECT_EQ(res->get_status_code(), 201);
    
  std::string resp_body = res->get_body();
  std::string id = "";
  std::size_t id_loc = resp_body.find("id");
  if(id_loc != std::string::npos)
  {
    id = resp_body.substr(id_loc + 6, 36);
  }

  EXPECT_NE(id, "");
    
  std::string new_request = "DELETE /memes/";
  new_request += "absolutegarbage";
  new_request += " HTTP/1.1\r\n\r\n";
    
  const int n2 = input.length();
  char char_arr2[n2 + 1];
  strcpy(char_arr2, new_request.c_str());
  req_char_arr = char_arr2;
  req_len = std::strlen(char_arr2);
  std::shared_ptr<request> req2(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out2 = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res2 = out->handle_request(req2);

  EXPECT_EQ(res2->get_status_code(), 400);
}

//simple test if search function works
TEST_F(MemeHandlerTest, SearchTest) {
  std::string input = "POST /memes HTTP/1.1\r\nHost: localhost:8080\r\nConnection: keep-alive\r\nContent-Type: application/x-www-form-urlencoded\r\nCache-Control: max-age=0\r\nContent-Length: 54\r\n\r\nmemeSelect=grumpy.jpg&topCaption=vdank&bottomCaption=9000";
  const int n = input.length();
  char char_arr[n+1];
  strcpy(char_arr, input.c_str());
  req_char_arr = char_arr;
  req_len = std::strlen(char_arr);
  std::shared_ptr<request> req(new request(req_char_arr, req_len));

  std::shared_ptr<route_handler> out = meme_handler::create_handler(config, "");
  std::shared_ptr<response> res = out->handle_request(req);

  std::string firstID = res->get_body().substr(7, 38);
  
  EXPECT_EQ(res->get_status_code(), 201);
  EXPECT_EQ(res->get_header("Content-Type"), "application/json");

  std::string input2 = "GET /memes/search/9000 HTTP/1.1\r\n\r\n";
  const int n2 = input2.length();
  char char_arr2[n2+1];
  strcpy(char_arr2, input2.c_str());
  req_char_arr = char_arr2;
  req_len = std::strlen(char_arr2);
  std::shared_ptr<request> req2(new request(req_char_arr, req_len));

  std::shared_ptr<response> res2 = out->handle_request(req2);

  EXPECT_THAT(res2->get_body(), HasSubstr(firstID));

  std::string input3 = "GET /memes/search/vdank HTTP/1.1\r\n\r\n";
  const int n3 = input3.length();
  char char_arr3[n3+1];
  strcpy(char_arr3, input3.c_str());
  req_char_arr = char_arr3;
  req_len = std::strlen(char_arr3);
  std::shared_ptr<request> req3(new request(req_char_arr, req_len));

  std::shared_ptr<response> res3 = out->handle_request(req3);

  EXPECT_THAT(res3->get_body(), HasSubstr(firstID));
}