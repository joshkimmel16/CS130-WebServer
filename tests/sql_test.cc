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

//insert into table test
TEST_F(SQLTest, SimpleInsert) {
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

  query_params pi;
  pi.push("id", "12345");
  pi.push("img_path", "img.jpg");
  bool o3 = sm->insert_record(table_name, pi);
  EXPECT_TRUE(o3);

  bool o4 = sm->drop_table(table_name);
  EXPECT_TRUE(o4);
    
  bool o5 = sm->disconnect();
  EXPECT_TRUE(o5);
}

//select from table test
TEST_F(SQLTest, SimpleSelect) {
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

  query_params pi;
  pi.push("id", "12345");
  pi.push("img_path", "img.jpg");
  bool o3 = sm->insert_record(table_name, pi);
  EXPECT_TRUE(o3);

  std::vector<std::vector<std::string>> result = sm->select_record(table_name, pi);
  EXPECT_EQ(result[0][0], "12345");
  EXPECT_EQ(result[0][1], "img.jpg");

  bool o4 = sm->drop_table(table_name);
  EXPECT_TRUE(o4);
    
  bool o5 = sm->disconnect();
  EXPECT_TRUE(o5);
}

//update a record from table test
TEST_F(SQLTest, SimpleUpdate) {
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

  query_params pi;
  pi.push("id", "12345");
  pi.push("img_path", "img.jpg");
  bool o3 = sm->insert_record(table_name, pi);
  EXPECT_TRUE(o3);

  std::vector<std::vector<std::string>> result = sm->select_record(table_name, pi);
  EXPECT_EQ(result[0][0], "12345");
  EXPECT_EQ(result[0][1], "img.jpg");

  query_params pset;
  pset.push("id", "54321");
  query_params pwhere;
  pwhere.push("id", "12345");
  bool o4 = sm->update_record(table_name, pset, pwhere);
  EXPECT_TRUE(o4);

  pset.push("img_path", "img.jpg");
  result = sm->select_record(table_name, pset);
  EXPECT_EQ(result[0][0], "54321");
  EXPECT_EQ(result[0][1], "img.jpg");

  bool o5 = sm->drop_table(table_name);
  EXPECT_TRUE(o5);
    
  bool o6 = sm->disconnect();
  EXPECT_TRUE(o6);
}

//delete a record from table test
TEST_F(SQLTest, SimpleDelete) {
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

  query_params pi;
  pi.push("id", "12345");
  pi.push("img_path", "img.jpg");
  bool o3 = sm->insert_record(table_name, pi);
  EXPECT_TRUE(o3);

  std::vector<std::vector<std::string>> result = sm->select_record(table_name, pi);
  EXPECT_EQ(result[0][0], "12345");
  EXPECT_EQ(result[0][1], "img.jpg");

  bool o4 = sm->delete_record(table_name, pi);
  EXPECT_TRUE(o4);

  result = sm->select_record(table_name, pi);
  EXPECT_EQ(result.empty(), true);

  bool o5 = sm->drop_table(table_name);
  EXPECT_TRUE(o5);
    
  bool o6 = sm->disconnect();
  EXPECT_TRUE(o6);
}

