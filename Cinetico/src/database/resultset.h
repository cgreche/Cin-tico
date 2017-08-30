#pragma once

#ifndef __RESULTSET_H__
#define __RESULTSET_H__

class Database;

#include <vector>

class ResultSet
{
	friend class SQLStatement;
	const Database &m_db;
	const SQLStatement &m_stmt;
	sqlite3_stmt *m_internalStmt;
	
	ResultSet(const Database &db, const SQLStatement &stmt);

public:
	void reset();
	bool next();
	int getInt(int colIndex);
	std::string getString(int colIndex);
	int close();
};

#endif