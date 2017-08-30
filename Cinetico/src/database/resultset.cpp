
#include "database.h"
#include "resultset.h"


ResultSet::ResultSet(const Database &db, const SQLStatement &stmt)
	: m_db(db), m_stmt(stmt) {
	m_internalStmt = NULL;
}

void ResultSet::reset() {
	sqlite3_reset(m_internalStmt);
}

bool ResultSet::next() {
	int rc;
	rc = sqlite3_step(m_internalStmt);

	if(rc == 21) {
		const char *errMessage = sqlite3_errmsg(m_db.internalDB());
		return rc;
	}

	return rc == SQLITE_ROW;
}

int ResultSet::getInt(int colIndex) {
	return sqlite3_column_int(m_internalStmt, colIndex);
}

std::string ResultSet::getString(int colIndex) {
	const char *text = (const char *)sqlite3_column_text(m_internalStmt, colIndex);
	std::string ret = text;
	return ret;
}

int ResultSet::close()
{
	delete this;
	return SQLITE_OK;
}