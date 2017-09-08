#pragma once

#ifndef __SQLITEDATABASE_H__
#define __SQLITEDATABASE_H__

#include <string>

#include <sqlite3.h>
#include "../database.h"

class SQLiteDatabase : public Database
{
	sqlite3 *m_internalDB;

public:
	SQLiteDatabase(const char *dbName);
	~SQLiteDatabase();
	virtual int open();
	virtual int close();
	virtual SQLStatement *prepare(const char *stmt);
	sqlite3* internalDB() const { return m_internalDB; }
};

#endif