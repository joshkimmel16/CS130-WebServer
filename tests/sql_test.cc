#include <cstdlib>
#include <cstring>
#include <iostream>
#include "gtest/gtest.h"
#include "sql_lib.h"

class SQLTest : public ::testing::Test {
  protected:
    std::string db_name;
};

//simple connect/disconnect test
TEST_F(SQLTest, SimpleConnect) {
  db_name = "test.db";
  
  std::unique_ptr<sql_manager> sm(new sql_manager(db_name));
  bool o1 = sm->connect();
  EXPECT_TRUE(o1);
  bool o2 = sm->disconnect();
  EXPECT_TRUE(o2);
}

//create/drop table test
TEST_F(SQLTest, SimpleTable) {
  db_name = "test.db";
  
  std::unique_ptr<sql_manager> sm(new sql_manager(db_name));
  bool o1 = sm->connect();
  EXPECT_TRUE(o1);
  
  std::string table_name = "TESTTABLE";
  query_params ps;
  ps.push("id", "TEXT PRIMARY KEY NOT NULL");
  ps.push("img_path", "TEXT NOT NULL");
    
  bool o2 = sm->create_table(table_name, ps);
  EXPECT_TRUE(o2);
  bool o3 = sm->drop_table(table_name);
  EXPECT_TRUE(o3);
    
  bool o4 = sm->disconnect();
  EXPECT_TRUE(o4);
}