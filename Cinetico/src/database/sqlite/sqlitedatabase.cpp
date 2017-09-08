
#include "sqlitedatabase.h"
#include "sqlitestatement.h"

SQLiteDatabase::SQLiteDatabase(const char *dbName)
	: Database(dbName)
{
	m_isOpen = false;
}

int SQLiteDatabase::open() {
	int rc = sqlite3_open(m_dbName.c_str(), &m_internalDB);
	m_isOpen = rc == SQLITE_OK;
	return rc;
}

int SQLiteDatabase::close() {
	int rc = sqlite3_close(m_internalDB);
	m_isOpen = false;
	delete this;
	return rc;
}

SQLiteDatabase::~SQLiteDatabase() {
	if(m_isOpen)
		close();
}

SQLStatement* SQLiteDatabase::prepare(const char *sqlStmt) {
	int rc;
	sqlite3_stmt *internalStmt;
	rc = sqlite3_prepare_v2(m_internalDB, sqlStmt, ::strlen(sqlStmt), &internalStmt, NULL);
	SQLiteStatement *stmt = new SQLiteStatement(*this,sqlStmt);
	stmt->m_internalStmt = internalStmt;
	return stmt;
}