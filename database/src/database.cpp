#include "database.hpp"

#include <exception>

#include <sqlite3.h>

Database::Database()
{}

DatabaseSqlite3::DatabaseSqlite3() :
    Database(),
    privData(NULL),
    connected(false)
{
    
}

void DatabaseSqlite3::open(std::string connectionString)
{
    sqlite3 *db;
    int rc = sqlite3_open(connectionString.c_str(), &db);
    privData = (void*)db;
    connected = true;
    
    if(rc)
    {
        close();
        throw std::runtime_error("Failed to open database");
    }
}

void DatabaseSqlite3::close()
{
    sqlite3 *db = (sqlite3*)privData;
    sqlite3_close(db);
    privData = NULL;
    connected = false;
}

bool DatabaseSqlite3::isConnected()
{
    return connected;
}

void DatabaseSqlite3::execute(std::string sqlString)
{
    sqlite3 *db = (sqlite3*)privData;
    int rc = sqlite3_exec(db, sqlString.c_str(), NULL, 0, NULL);
    if( rc != SQLITE_OK ){
        throw std::runtime_error("Failed to execute query");
    }
}
