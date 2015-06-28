#include "database.hpp"

#include <string.h>

#include <exception>

#include <sqlite3.h>

namespace Tracker
{
namespace Database
{

Database::Database()
{}

Database::~Database()
{}

Database::DatabaseException::DatabaseException(std::string msg) :
	msg(msg)
{}

const char* Database::DatabaseException::what() const throw()
{
	return msg.c_str();
}

DatabaseSqlite3::DatabaseSqlite3() :
    Database(),
    privData(NULL),
    connected(false)
{}

DatabaseSqlite3::~DatabaseSqlite3()
{}

void DatabaseSqlite3::open(std::string connectionString)
{
    sqlite3 *db;
    int rc = sqlite3_open(connectionString.c_str(), &db);
    privData = (void*)db;
    connected = true;
    
    if(rc)
    {
        close();
        throw DatabaseException("Failed to open database");
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
        throw DatabaseException("Failed to execute query");
    }
}

std::shared_ptr<std::vector<std::vector<std::string>>> DatabaseSqlite3::select(std::string sqlString)
{
    auto returnValue = std::make_shared<std::vector<std::vector<std::string>>>();
    sqlite3 *db = (sqlite3*)privData;
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db,sqlString.c_str(), -1, &res, 0);
    
    if (rc != SQLITE_OK) {
        throw DatabaseException("Failed to execute query");
    }
    
    unsigned int columnCount = sqlite3_column_count(res);
    
    do
    {
        rc = sqlite3_step(res);
        
        if(rc == SQLITE_ROW)
        {
            auto newRow = std::vector<std::string>();
            
            for(unsigned int i = 0; i < columnCount; i++)
            {
                const unsigned char * colResult = sqlite3_column_text(res, i);
                newRow.push_back(std::string((const char*)colResult,strlen((const char*)colResult)));                                
            }
            
            returnValue->push_back(newRow);
        }
        
    }while(rc == SQLITE_ROW);
    
    sqlite3_finalize(res);
    
    return returnValue;
}
    
uint64_t DatabaseSqlite3::lastInsertRowId()
{
    return sqlite3_last_insert_rowid((sqlite3*)privData);
}

}
}
