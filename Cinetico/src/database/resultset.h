#pragma once

#ifndef __RESULTSET_H__
#define __RESULTSET_H__

class Database;

#include <vector>

class ResultSet
{
	friend class SQLStatement;

protected:
	const Database &m_db;
	const SQLStatement &m_stmt;
	
	ResultSet(const Database &db, const SQLStatement &stmt)
		: m_db(db), m_stmt(stmt) {
	}

public:
	virtual void reset() = 0;
	virtual bool next() = 0;
	virtual int getInt(int colIndex) = 0;
	virtual std::string getString(int colIndex) = 0;
	virtual bool isNull(int colIndex) = 0;
	virtual int close() = 0;
};

#endif