
#include "database.h"
#include "SQLStatement.h"

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
	int rc;
	const char *errMessage;
	do {
		rc = sqlite3_step(m_internalStmt);
		if(rc == 100) {
			const char *col1 = (const char *)sqlite3_column_text(m_internalStmt, 0);
			const char *col2 = (const char *)sqlite3_column_text(m_internalStmt, 1);
			const char *col3 = (const char *)sqlite3_column_text(m_internalStmt, 2);
			const char *col4 = (const char *)sqlite3_column_text(m_internalStmt, 3);
		}

		if(rc == 21) {
			errMessage = sqlite3_errmsg(m_db.internalDB());
			return rc;
		}
	} while(rc != SQLITE_DONE);

	sqlite3_reset(m_internalStmt);

	return SQLITE_OK;
}