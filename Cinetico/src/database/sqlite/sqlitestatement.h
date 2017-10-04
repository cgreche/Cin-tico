#pragma once

#ifndef __SQLITESTATEMENT_H__
#define __SQLITESTATEMENT_H__

#include "../sqlstatement.h"

class Database;
class ResultSet;

#include <vector>

class SQLiteStatement : public SQLStatement
{
	friend class SQLiteDatabase;

protected:
	sqlite3_stmt *m_internalStmt;
	
	SQLiteStatement(const SQLiteDatabase &db, const char *stmt);

public:
	virtual int bind(int paramIndex, int value);
	virtual int bind(int paramIndex, unsigned int value);
	virtual int bind(int paramIndex, const char *value);
	virtual int bind(int paramIndex, const std::string &value);
	virtual int bind(int paramIndex, float value);
	virtual int bindNull(int paramIndex);
	virtual int execute();
	virtual ResultSet *query();
	virtual int close();
};

#endif