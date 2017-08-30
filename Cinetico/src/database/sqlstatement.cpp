
#include "database.h"
#include "sqlstatement.h"
#include "resultset.h"

SQLStatement::SQLStatement(const Database &db)
	: m_db(db) {

}

int SQLStatement::bind(int paramIndex, int value) {
	return sqlite3_bind_int(m_internalStmt, paramIndex, value);
}

int SQLStatement::bind(int paramIndex, const char *value) {
	return sqlite3_bind_text(m_internalStmt, paramIndex, value, ::strlen(value), SQLITE_STATIC);
}

int SQLStatement::bind(int paramIndex, const std::string &value) {
	//SQLITE_STATIC;
	const char *cstr = value.c_str();
	size_t len = value.length();
	return sqlite3_bind_text(m_internalStmt, paramIndex, cstr, len, NULL);
}

int SQLStatement::close() {
	int rc;
	rc = sqlite3_finalize(m_internalStmt);
	delete this;
	return rc;
}

int SQLStatement::execute() {
	int rc = sqlite3_step(m_internalStmt);
	sqlite3_reset(m_internalStmt);
	return rc == SQLITE_DONE;
}

ResultSet *SQLStatement::query() {
	ResultSet * ret = new ResultSet(m_db,*this);
	ret->m_internalStmt = m_internalStmt;
	return ret;
}