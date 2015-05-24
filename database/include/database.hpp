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

#include <string>

class Database
{
public:
    Database();
    virtual void open(std::string connectionString) = 0;
    virtual void close() = 0;
    virtual bool isConnected() = 0;
};

class DatabaseSqlite3 : Database
{
public:
    DatabaseSqlite3();
    void open(std::string connectionString) override;
    void close() override;
    bool isConnected() override;
private:
    void * privData;
    bool connected;
};

#endif
