#pragma once

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <string>

#include "lib/sqlite-amalgamation-3200100/sqlite3.h"

#include "sqlstatement.h"

class Database
{
	sqlite3 *m_internalDB;

	std::string m_dbName;

	bool m_isOpen;
public:
	Database(std::string dbName);
	~Database();
	int open();
	int close();
	SQLStatement *prepare(const std::string &stmt);

	bool isOpen() const { return m_isOpen; }

	sqlite3* internalDB() const { return m_internalDB; }
};

#endif