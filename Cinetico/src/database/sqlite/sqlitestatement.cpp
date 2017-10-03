
#include "sqlitedatabase.h"
#include "sqlitestatement.h"
#include "sqliteresultset.h"

SQLiteStatement::SQLiteStatement(const SQLiteDatabase &db, const char *stmt)
	: SQLStatement(db,stmt) {

}

int SQLiteStatement::bind(int paramIndex, int value) {
	return sqlite3_bind_int(m_internalStmt, paramIndex, value);
}

int SQLiteStatement::bind(int paramIndex, const char *value) {
	return sqlite3_bind_text(m_internalStmt, paramIndex, value, ::strlen(value), SQLITE_STATIC);
}

int SQLiteStatement::bind(int paramIndex, const std::string &value) {
	//SQLITE_STATIC;
	const char *cstr = value.c_str();
	size_t len = value.length();
	return sqlite3_bind_text(m_internalStmt, paramIndex, cstr, len, NULL);
}

int SQLiteStatement::bindNull(int paramIndex) {
	return sqlite3_bind_null(m_internalStmt, paramIndex);
}

int SQLiteStatement::close() {
	int rc;
	rc = sqlite3_finalize(m_internalStmt);
	delete this;
	return rc;
}

int SQLiteStatement::execute() {
	int rc = sqlite3_step(m_internalStmt);
	sqlite3_reset(m_internalStmt);
	return rc == SQLITE_DONE;
}

ResultSet *SQLiteStatement::query() {
	SQLiteResultSet *ret = new SQLiteResultSet((SQLiteDatabase&)m_db,*this);
	ret->m_internalStmt = m_internalStmt;
	return ret;
}