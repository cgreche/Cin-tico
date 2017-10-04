#pragma once

#ifndef __DATABASE_H__
#define __DATABASE_H__

#include <string>

#include <sqlite3.h>

#include "sqlstatement.h"
#include "resultset.h"

class Database
{
protected:
	std::string m_dbName;
	bool m_isOpen;

public:
	Database(const char *dbName) {
		m_isOpen = false;
		m_dbName = dbName;
	}

	virtual ~Database() {

	}

	virtual int open() = 0;
	virtual int close() = 0;
	virtual SQLStatement *prepare(const char *stmt) = 0;
	virtual int getLastRowId() = 0;

	bool isOpen() const { return m_isOpen; }
};

#endif