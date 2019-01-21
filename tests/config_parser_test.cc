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
}

TEST_F(NginxConfigParserTest, TokenConfig) {
  bool success = parser.Parse("token_config", &out_config);

  EXPECT_TRUE(success);
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
}

TEST_F(NginxConfigParserTest, ComboConfig) {
  bool success = parser.Parse("combo_config", &out_config);

  EXPECT_TRUE(success);
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