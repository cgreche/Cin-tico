
#include "database.h"
#include "sqlstatement.h"

Database::Database(const char *dbName) {
	m_dbName = dbName;
	m_isOpen = false;
}

int Database::open() {
	int rc = sqlite3_open(m_dbName.c_str(), &m_internalDB);
	m_isOpen = rc == SQLITE_OK;
	return rc;
}

int Database::close() {
	int rc = sqlite3_close(m_internalDB);
	m_isOpen = false;
	return rc;
}

Database::~Database() {
	if(m_isOpen)
		close();
}

SQLStatement* Database::prepare(const char *sqlStmt) {
	int rc;
	sqlite3_stmt *internalStmt;
	rc = sqlite3_prepare_v2(m_internalDB, sqlStmt, ::strlen(sqlStmt), &internalStmt, NULL);
	SQLStatement *stmt = new SQLStatement(*this);
	stmt->m_stmtStr = sqlStmt;
	stmt->m_internalStmt = internalStmt;
	return stmt;
}