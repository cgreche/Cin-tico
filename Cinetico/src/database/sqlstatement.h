#pragma once

#ifndef __SQLSTATEMENT_H__
#define __SQLSTATEMENT_H__

class Database;
class ResultSet;

#include <vector>

class SQLStatement
{
	friend class Database;

protected:
	const Database &m_db;
	std::string m_stmtStr;
	
	SQLStatement(const Database &db, const char *stmt)
		: m_db(db) {
		m_stmtStr = stmt;
	}

public:
	virtual int bind(int paramIndex, int value) = 0;
	virtual int bind(int paramIndex, unsigned int value) = 0;
	virtual int bind(int paramIndex, const char *value) = 0;
	virtual int bind(int paramIndex, const std::string &value) = 0;
	virtual int bind(int paramIndex, float value) = 0;
	virtual int bindNull(int paramIndex) = 0;
	virtual int execute() = 0;
	virtual ResultSet *query() = 0;
	virtual int close() = 0;
};

#endif