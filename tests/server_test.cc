#include <cstdlib>
#include <cstring>
#include "server.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

class MockIO : public boost::asio::io_service {
};


class ServerTest : public ::testing::Test {
  protected:
    short port;
    unsigned int threads;
    MockIO service;
    NginxConfigParser parser;
    NginxConfig out_config;
    std::shared_ptr<NginxConfig> config;
};


TEST_F(ServerTest, ValidRouter) {
	parser.Parse("server/router_config", &out_config);
    out_config.ParseStatements();
    port = out_config.GetPort();
    threads = out_config.GetThreadCount();
	std::unique_ptr<server> serv(new server(service, out_config, port, threads));
	EXPECT_TRUE(serv->create_router("/server/path"));
    EXPECT_TRUE(serv->register_route("/test", "testing"));
    EXPECT_TRUE(serv->register_default_header("header", "value"));
    EXPECT_TRUE(serv->start_accept());
    EXPECT_TRUE(serv->get_status());
}

TEST_F(ServerTest, StopPool) {
	parser.Parse("server/router_config", &out_config);
    out_config.ParseStatements();
    port = out_config.GetPort();
    threads = out_config.GetThreadCount();
	std::unique_ptr<server> serv(new server(service, out_config, port, threads));
	EXPECT_TRUE(serv->create_router("/server/path"));
    EXPECT_TRUE(serv->register_route("/test", "testing"));
    EXPECT_TRUE(serv->register_default_header("header", "value"));
    EXPECT_TRUE(serv->start_accept());
    EXPECT_TRUE(serv->stop_all_sessions());
}