/*
 *  database.h
 *  database
 *
 *  Created by Alex Green on 2015-05-24.
 *  Copyright (c) 2015 test. All rights reserved.
 *
 */

#ifndef __DATABASE_HPP__
#define __DATABASE_HPP__

#include <memory>
#include <string>
#include <vector>

namespace Tracker
{
namespace Database
{

class Database
{
public:
    Database();
    virtual ~Database();
    virtual void open(std::string connectionString) = 0;
    virtual void close() = 0;
    virtual bool isConnected() = 0;
    virtual void execute(std::string sqlString) = 0;
    virtual std::shared_ptr<std::vector<std::vector<std::string>>> select(std::string sqlString) = 0;
};

class DatabaseSqlite3 : public Database
{
public:
    DatabaseSqlite3();
    ~DatabaseSqlite3() override;
    void open(std::string connectionString) override;
    void close() override;
    bool isConnected() override;
    void execute(std::string sqlString) override;
    std::shared_ptr<std::vector<std::vector<std::string>>> select(std::string sqlString) override;
private:
    void * privData;
    bool connected;
};
    
}
}

#endif
