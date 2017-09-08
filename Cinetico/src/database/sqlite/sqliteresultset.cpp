
#include "sqlitedatabase.h"
#include "sqlitestatement.h"
#include "sqliteresultset.h"


SQLiteResultSet::SQLiteResultSet(const SQLiteDatabase &db, const SQLiteStatement &stmt)
	: ResultSet(db,stmt) {
	m_internalStmt = NULL;
}

void SQLiteResultSet::reset() {
	sqlite3_reset(m_internalStmt);
}

bool SQLiteResultSet::next() {
	int rc;
	rc = sqlite3_step(m_internalStmt);

	if(rc == 21) {
		const char *errMessage = sqlite3_errmsg(((SQLiteDatabase&)m_db).internalDB());
		return rc;
	}

	return rc == SQLITE_ROW;
}

int SQLiteResultSet::getInt(int colIndex) {
	return sqlite3_column_int(m_internalStmt, colIndex);
}

std::string SQLiteResultSet::getString(int colIndex) {
	const char *text = (const char *)sqlite3_column_text(m_internalStmt, colIndex);
	std::string ret = text;
	return ret;
}

int SQLiteResultSet::close()
{
	delete this;
	return SQLITE_OK;
}