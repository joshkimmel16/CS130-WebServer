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

//insert duplicate id into table test when id is primary key
TEST_F(SQLTest, InsertDuplicatePrimaryKey) {
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

  query_params pi2;
  pi.push("id", "12345");
  pi.push("img_path", "another.jpg");
  bool o4 = sm->insert_record(table_name, pi2);
  EXPECT_FALSE(o4);

  query_params pid;
  pi.push("id", "12345");
  std::vector<std::vector<std::string>> result = sm->select_record(table_name, pid);
  EXPECT_EQ(result[0][0], "12345");
  EXPECT_EQ(result[0][1], "img.jpg");

  bool o5 = sm->drop_table(table_name);
  EXPECT_TRUE(o5);
    
  bool o6 = sm->disconnect();
  EXPECT_TRUE(o6);
}

//insert value for nonexistent field into table test
TEST_F(SQLTest, InsertNonexistentField) {
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
  pi.push("wrong", "img.jpg");
  bool o3 = sm->insert_record(table_name, pi);
  EXPECT_FALSE(o3);

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

//select when value is a SQL injection from table test
TEST_F(SQLTest, SelectSQLInjection) {
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

  query_params psi;
  pi.push("id", "111 OR 1=1");
  result = sm->select_record(table_name, pi);
  EXPECT_TRUE(result.empty());

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

//update value for nonexistent field from table test
TEST_F(SQLTest, UpdateNonexistentField) {
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
  pset.push("something", "54321");
  query_params pwhere;
  pwhere.push("id", "12345");
  bool o4 = sm->update_record(table_name, pset, pwhere);
  EXPECT_FALSE(o4);

  result = sm->select_record(table_name, pwhere);
  EXPECT_EQ(result[0][0], "12345");
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

//delete a nonexistent record from table test
TEST_F(SQLTest, DeleteNonexistentRecord) {
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

  query_params pe;
  pe.push("id", "54321");
  pe.push("img_path", "img.jpg");
  std::vector<std::vector<std::string>> result = sm->select_record(table_name, pe);
  EXPECT_TRUE(result.empty());

  bool o4 = sm->delete_record(table_name, pe);
  EXPECT_TRUE(o4);

  bool o5 = sm->drop_table(table_name);
  EXPECT_TRUE(o5);
    
  bool o6 = sm->disconnect();
  EXPECT_TRUE(o6);
}

//search from table test
TEST_F(SQLTest, SimpleSearch) {
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

  query_params pi2;
  pi2.push("id", "5555");
  pi2.push("img_path", "img.jpg");
  bool o4 = sm->insert_record(table_name, pi2);
  EXPECT_TRUE(o4);

  query_params pi3;
  pi3.push("id", "43445");
  pi3.push("img_path", "something.jpg");
  bool o5 = sm->insert_record(table_name, pi3);
  EXPECT_TRUE(o5);

  query_params pi4;
  std::vector<std::vector<std::string>> result = sm->select_record(table_name, pi4);
  EXPECT_EQ(result[0][0], "12345");
  EXPECT_EQ(result[0][1], "img.jpg");
  EXPECT_EQ(result[1][0], "5555");
  EXPECT_EQ(result[1][1], "img.jpg");
  EXPECT_EQ(result[2][0], "43445");
  EXPECT_EQ(result[2][1], "something.jpg");

  query_params pf;
  pf.push("img_path", "img");
  result = sm->search_record(table_name, pf);
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[0][0], "12345");
  EXPECT_EQ(result[0][1], "img.jpg");
  EXPECT_EQ(result[1][0], "5555");
  EXPECT_EQ(result[1][1], "img.jpg");

  query_params pf2;
  pf2.push("img_path", "something");
  result = sm->search_record(table_name, pf2);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0][0], "43445");
  EXPECT_EQ(result[0][1], "something.jpg");

  bool o6 = sm->drop_table(table_name);
  EXPECT_TRUE(o6);
    
  bool o7 = sm->disconnect();
  EXPECT_TRUE(o7);
}

//search from table test but no matches
TEST_F(SQLTest, SearchNoMatches) {
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

  query_params pf;
  pf.push("img_path", "something");
  result = sm->search_record(table_name, pf);
  EXPECT_TRUE(result.empty());

  bool o4 = sm->drop_table(table_name);
  EXPECT_TRUE(o4);
    
  bool o5 = sm->disconnect();
  EXPECT_TRUE(o5);
}

//search from table test with longer phrase
TEST_F(SQLTest, SearchLongerPhrase) {
  db_name = "test.db";
  
  std::unique_ptr<sql_manager> sm(new sql_manager(db_name));
  bool o1 = sm->connect();
  EXPECT_TRUE(o1);
  
  std::string table_name = "TESTTABLE";
  query_params ps;
  ps.push("id", "TEXT PRIMARY KEY NOT NULL");
  ps.push("img_desc", "TEXT NOT NULL");
  bool o2 = sm->create_table(table_name, ps);
  EXPECT_TRUE(o2);

  query_params pi;
  pi.push("id", "12345");
  pi.push("img_desc", "Hi There! How are you doing today?");
  bool o3 = sm->insert_record(table_name, pi);
  EXPECT_TRUE(o3);

  query_params pi2;
  pi2.push("id", "55955");
  pi2.push("img_desc", "What's up?! How's it going?");
  bool o4 = sm->insert_record(table_name, pi2);
  EXPECT_TRUE(o4);

  query_params pi3;
  pi3.push("id", "43445");
  pi3.push("img_desc", "Hey. How are your dogs?");
  bool o5 = sm->insert_record(table_name, pi3);
  EXPECT_TRUE(o5);

  query_params pi4;
  std::vector<std::vector<std::string>> result = sm->select_record(table_name, pi4);
  EXPECT_EQ(result.size(), 3);
  EXPECT_EQ(result[0][0], "12345");
  EXPECT_EQ(result[0][1], "Hi There! How are you doing today?");
  EXPECT_EQ(result[1][0], "55955");
  EXPECT_EQ(result[1][1], "What's up?! How's it going?");
  EXPECT_EQ(result[2][0], "43445");
  EXPECT_EQ(result[2][1], "Hey. How are your dogs?");

  query_params pf;
  pf.push("img_desc", "how are");
  result = sm->search_record(table_name, pf);
  EXPECT_EQ(result.size(), 2);
  EXPECT_EQ(result[0][0], "12345");
  EXPECT_EQ(result[0][1], "Hi There! How are you doing today?");
  EXPECT_EQ(result[1][0], "43445");
  EXPECT_EQ(result[1][1], "Hey. How are your dogs?");

  query_params pf2;
  pf2.push("id", "9");
  result = sm->search_record(table_name, pf2);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result[0][0], "55955");
  EXPECT_EQ(result[0][1], "What's up?! How's it going?");

  query_params pf3;
  pf3.push("id", "9");
  pf3.push("img_desc", "how are");
  result = sm->search_record(table_name, pf3);
  EXPECT_EQ(result.size(), 3);

  bool o6 = sm->drop_table(table_name);
  EXPECT_TRUE(o6);
    
  bool o7 = sm->disconnect();
  EXPECT_TRUE(o7);
}