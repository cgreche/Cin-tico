#pragma once

#ifndef __SQLITERESULTSET_H__
#define __SQLITERESULTSET_H__

#include "../resultset.h"

class Database;
class SQLiteStatement;

#include <vector>

class SQLiteResultSet : public ResultSet
{
	friend class SQLiteStatement;
	sqlite3_stmt *m_internalStmt;
	
	SQLiteResultSet(const SQLiteDatabase &db, const SQLiteStatement &stmt);

public:
	virtual void reset();
	virtual bool next();
	virtual int getInt(int colIndex);
	virtual std::string getString(int colIndex);
	virtual float getFloat(int colIndex);
	virtual bool isNull(int colIndex);
	virtual int close();
};

#endif