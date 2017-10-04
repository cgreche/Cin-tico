
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
	return text ? text : "";
}

float SQLiteResultSet::getFloat(int colIndex) {
	return (float)sqlite3_column_double(m_internalStmt, colIndex);
}

bool SQLiteResultSet::isNull(int colIndex) {
	return sqlite3_column_type(m_internalStmt, colIndex) == SQLITE_NULL;
}

int SQLiteResultSet::close()
{
	delete this;
	return SQLITE_OK;
}