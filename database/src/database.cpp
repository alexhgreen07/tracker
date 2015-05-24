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
    int rc;
    rc = sqlite3_open(connectionString.c_str(), &db);
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
