//A set of functions to deal with SQLite operations
#ifndef SQLLIB_H
#define SQLLIB_H

#include <iostream>
#include <string>
#include <cstdlib>
#include <vector>
#include <sqlite3.h>

struct query_params 
{
    std::vector<std::pair<std::string, std::string>> selectors;
    void push (std::string key, std::string value);
};

class sql_manager
{
public:
    sql_manager(std::string db_name); //constructor
    bool connect(); //connect to a SQLite instance
    bool disconnect(); //disconnect from a SQLite instance
    bool create_table(std::string table_name, query_params table_params); //create a new table if it doesn't exist
    bool drop_table(std::string table_name); //drop the given table if it exists
private:
    std::string db_name_;
    sqlite3* db_;
    bool is_connected_;
};

#endif