
#include "sql_lib.h"

//add a query param
void query_params::push (std::string key, std::string value)
{
    std::pair<std::string, std::string> toAdd(key, value);
    selectors.push_back(toAdd);
}

//constructor takes a name for the database to connect to
sql_manager::sql_manager (std::string db_name)
{
    db_name_ = db_name;
}

//open a connection to the internal database
bool sql_manager::connect ()
{
    int rc = sqlite3_open(db_name_.c_str(), &db_);
    if (rc) { return false; }
    else { return true; }
}

//open a connection to the internal database
bool sql_manager::disconnect ()
{
    int rc = sqlite3_close(db_);
    if (rc) { return false; }
    else { return true; }
}

//create a table if it doesn't already exist
bool sql_manager::create_table(std::string table_name, query_params table_params)
{
    std::string q = "CREATE TABLE IF NOT EXISTS ";
    q += (table_name + "(");
    for (unsigned int i=0; i<table_params.selectors.size(); i++)
    {
        q += (table_params.selectors[i].first + " " + table_params.selectors[i].second);
        if (i != table_params.selectors.size() - 1) { q += ","; }
        else { q += ");"; }
    }
    
    char* err;
    int rc = sqlite3_exec(db_, q.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) { return false; }
    else { return true; }
}

//drop the given table if it exists
bool sql_manager::drop_table(std::string table_name)
{
    std::string q = "DROP TABLE IF EXISTS " + table_name + ";";
    
    char* err;
    int rc = sqlite3_exec(db_, q.c_str(), NULL, NULL, &err);
    if (rc != SQLITE_OK) { return false; }
    else { return true; }
}