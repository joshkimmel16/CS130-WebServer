#include "gtest/gtest.h"
#include "config_parser.h"

class NginxConfigParserTest : public ::testing::Test {
  protected:
    NginxConfigParser parser;
    NginxConfig out_config;
};

TEST_F(NginxConfigParserTest, SimpleConfig) {
  bool success = parser.Parse("example_config", &out_config);

  EXPECT_TRUE(success);
  EXPECT_EQ(out_config.ToString(), "foo \"bar\";\nserver {\n  listen 80;\n  server_name foo.com;\n  root /home/ubuntu/sites/foo/;\n}\n");
}

TEST_F(NginxConfigParserTest, EmptyConfig) {
  bool success = parser.Parse("empty_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, NonexistentConfig) {
  bool success = parser.Parse("nonexistent_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, CommentConfig) {
  bool success = parser.Parse("comment_config", &out_config);
  
  EXPECT_TRUE(success);
  EXPECT_EQ(out_config.ToString(), 
    "server {\n  listen 80;\n  server_name foo.com;\n  root /home/ubuntu/sites/foo/;\n}\nsomething {\n  another thing;\n}\n");
}

TEST_F(NginxConfigParserTest, CommentOnlyConfig) {
   bool success = parser.Parse("comment_only_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, NoSemicolonConfig) {
  bool success = parser.Parse("no_semicolon_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, SemicolonConfig) {
  bool success = parser.Parse("semicolon_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, ColAfterBrackConfig) {
  bool success = parser.Parse("col_after_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, QuoteConfig) {
  bool success = parser.Parse("quote_config", &out_config);

  EXPECT_TRUE(success);
  EXPECT_EQ(out_config.ToString(), "user \"hello\";\nfoo 'b';\nfoo \"you're cool!\";\ncomplex \"wowowo342424 !~!#; { } hello '\";\n");
}

TEST_F(NginxConfigParserTest, TokenConfig) {
  bool success = parser.Parse("token_config", &out_config);

  EXPECT_TRUE(success);
  EXPECT_EQ(out_config.ToString(), 
    "user hello;\npid /some/path/to/this.pid;\nprocesses 4;\ntype text/plain;\nsend_timeout 3m;\ninclude something/another.types;\n");
}

TEST_F(NginxConfigParserTest, NoTokenColConfig) {
  bool success = parser.Parse("no_token_col_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, NoTokenBraceConfig) {
  bool success = parser.Parse("no_token_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, ColInBraceConfig) {
  bool success = parser.Parse("col_in_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EndBraceConfig) {
  bool success = parser.Parse("end_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, BeginBraceConfig) {
  bool success = parser.Parse("begin_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MismatchBraceConfig) {
  bool success = parser.Parse("mismatch_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, SpaceConfig) {
  bool success = parser.Parse("space_config", &out_config);

  EXPECT_TRUE(success);
  EXPECT_EQ(out_config.ToString(), "food fruits;\nfood dairy;\nfood junk;\nfood rice;\nnoodles {\n  type/soup pho;\n  type/dry yakisoba;\n  type/soup beef noodle soup;\n  type/soup ramen;\n}\nlots of space \"yay\";\nwooo;\nspace;\n");
}

TEST_F(NginxConfigParserTest, NestedConfig) {
  bool success = parser.Parse("nested_config", &out_config);

  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, MixedConfig) {
  bool success = parser.Parse("mixed_config", &out_config);

  EXPECT_TRUE(success);
}

TEST_F(NginxConfigParserTest, MixedQuoteConfig) {
  bool success = parser.Parse("mixed_quote_config", &out_config);

  EXPECT_TRUE(success);
  EXPECT_EQ(out_config.ToString(), "foo \"does this 'work'\";\nbar 'how about \"this\"';\ntool 'and ' how about this' yay';\n");
}

TEST_F(NginxConfigParserTest, MismatchSingleQuoteConfig) {
  bool success = parser.Parse("mismatch_single_quote_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, MismatchDoubleQuoteConfig) {
  bool success = parser.Parse("mismatch_double_quote_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, ComboConfig) {
  bool success = parser.Parse("combo_config", &out_config);

  EXPECT_TRUE(success);
  EXPECT_EQ(out_config.ToString(), "error_log logs/error.log;\npid logs/nginx.pid;\nworker_rlimit_nofile 8192;\nevents {\n  worker_connections 4096;\n}\nhttp {\n  include conf/mime.types;\n  include /etc/nginx/proxy.conf;\n  include /etc/nginx/fastcgi.conf;\n  index index.html index.htm index.php;\n  default_type application/octet-stream;\n  log_format main '$remote_addr - $remote_user [$time_local]  $status ' '\"$request\" $body_bytes_sent \"$http_referer\" ' '\"$http_user_agent\" \"$http_x_forwarded_for\"';\n  access_log logs/access.log main;\n  sendfile on;\n  tcp_nopush on;\n  server_names_hash_bucket_size 128;\n  server {\n    listen 80;\n    server_name domain1.com www.domain1.com;\n    access_log logs/domain1.access.log main;\n    root html;\n    location ~ \\.php$ {\n      fastcgi_pass 127.0.0.1:1025;\n    }\n  }\n}\n");
}

TEST_F(NginxConfigParserTest, EmptyBraceConfig) {
  bool success = parser.Parse("empty_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, EmptyNestedBraceConfig) {
  bool success = parser.Parse("empty_nested_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, CommentBraceConfig) {
  bool success = parser.Parse("comment_brace_config", &out_config);

  EXPECT_FALSE(success);
}

TEST_F(NginxConfigParserTest, PortConfig) {
  bool parse_config = parser.Parse("port_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_EQ(out_config.GetPort(), 80);
  EXPECT_EQ(out_config.ToString(), "port 80;\n");
  EXPECT_TRUE(parse_config);
  EXPECT_TRUE(parse_stmt);
}

TEST_F(NginxConfigParserTest, Port8080Config) {
  bool parse_config = parser.Parse("port_8080_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_EQ(out_config.GetPort(), 8080);
  EXPECT_EQ(out_config.ToString(), "port 8080;\n");
  EXPECT_TRUE(parse_config);
  EXPECT_TRUE(parse_stmt);
}

TEST_F(NginxConfigParserTest, BadPortConfig) {
  bool parse_config = parser.Parse("bad_port_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_TRUE(parse_config);
  EXPECT_FALSE(parse_stmt);

}

TEST_F(NginxConfigParserTest, BadPortNumConfig) {
  bool parse_config = parser.Parse("bad_port_num_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_TRUE(parse_config);
  EXPECT_FALSE(parse_stmt);

}

TEST_F(NginxConfigParserTest, NoPortNumConfig) {
  bool parse_config = parser.Parse("no_port_num_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_TRUE(parse_config);
  EXPECT_FALSE(parse_stmt);

}

TEST_F(NginxConfigParserTest, HighPortConfig) {
  bool parse_config = parser.Parse("high_port_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_TRUE(parse_config);
  EXPECT_FALSE(parse_stmt);

}

TEST_F(NginxConfigParserTest, RootPath) {
  bool parse_config = parser.Parse("root_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_TRUE(parse_config);
  EXPECT_TRUE(parse_stmt);
  EXPECT_EQ(out_config.GetServerRoot(), "/root/path");
}

TEST_F(NginxConfigParserTest, NoRootPath) {
  bool parse_config = parser.Parse("no_root_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_TRUE(parse_config);
  EXPECT_TRUE(parse_stmt);
  EXPECT_EQ(out_config.GetServerRoot(), "");
}

TEST_F(NginxConfigParserTest, CurrentDirRootPath) {
  bool parse_config = parser.Parse("current_root_config", &out_config);
  bool parse_stmt = out_config.ParseStatements();

  EXPECT_TRUE(parse_config);
  EXPECT_TRUE(parse_stmt);
  EXPECT_EQ(out_config.GetServerRoot(), "");
}