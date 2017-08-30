#pragma once

#ifndef __SQLSTATEMENT_H__
#define __SQLSTATEMENT_H__

class Database;
class ResultSet;

#include <vector>

class SQLStatement
{
	friend class Database;

	const Database &m_db;
	std::string m_stmtStr;
	sqlite3_stmt *m_internalStmt;
	
	SQLStatement(const Database &db);

public:
	int bind(int paramIndex, int value);
	int bind(int paramIndex, const char *value);
	int bind(int paramIndex, const std::string &value);
	int execute();
	ResultSet *query();
	int close();
};

#endif