
#include "database/database.h"
#include "UserProfileDAO.h"


UserProfileDAO::UserProfileDAO(Database &db)
	: m_db(db)
{

}

void UserProfileDAO::save(UserProfile &user) {
	const char *sql = "INSERT INTO USER_PROFILE(login_name,password,creation_date) VALUES(?,?,?);";

	SQLStatement *stmt;
	stmt = m_db.prepare(sql);
	stmt->bind(1, user.username()); //safer to use the created user properties
	stmt->bind(2, user.password());
	stmt->bind(3, user.creationDate());
	int rc = stmt->execute();
	if(rc != 0) {
		//todo
	}
	stmt->close();
}


void UserProfileDAO::update(UserProfile &user) {
	const char *sql = "UPDATE USER_PROFILE SET login_name = ?, password = ?, creation_date = ?;";
	SQLStatement *stmt;
	stmt = m_db.prepare(sql);
	stmt->bind(1, user.username());
	stmt->bind(2, user.password());
	stmt->bind(3, user.creationDate());
	int rc = stmt->execute();
	if(rc != 0) {
		//todo
	}
	stmt->close();
}

std::vector<UserProfile *> UserProfileDAO::getAll() {
	const char *sql = "SELECT * FROM USER_PROFILE;";
	SQLStatement *stmt;
	std::vector<UserProfile *> userList;
	stmt = m_db.prepare(sql);
	ResultSet *rs = stmt->query();
	if(rs) {
		while(rs->next()) {
			UserProfile *user = new UserProfile(rs->getString(0), rs->getString(1), rs->getInt(2));
			userList.push_back(user);
		}
		rs->close();
	}
	stmt->close();
	return userList;
}

UserProfile *UserProfileDAO::getByLoginName(const char *loginName) {
	const char *sql = "SELECT * FROM USER_PROFILE WHERE login_name = ?;";
	UserProfile *user = NULL;
	SQLStatement *stmt = m_db.prepare(sql);
	stmt->bind(1, loginName);
	ResultSet *rs = stmt->query();
	if(rs) {
		if(rs->next())
			user = new UserProfile(rs->getString(0), rs->getString(1), rs->getInt(2));
	}
	stmt->close();
	return user;
}

void UserProfileDAO::exclude(UserProfile &user) {
	const char *sql = "DELETE FROM USER_PROFILE WHERE login_name = ?;";
	SQLStatement *stmt;
	stmt = m_db.prepare(sql);
	stmt->bind(1, user.username());
	int rc = stmt->execute();
	if(rc != 0) {
		//todo
	}
	stmt->close();
}

void UserProfileDAO::exclude(const char *loginName) {
	const char *sql = "DELETE FROM USER_PROFILE WHERE login_name = ?;";
	SQLStatement *stmt;
	stmt = m_db.prepare(sql);
	stmt->bind(1, loginName);
	int rc = stmt->execute();
	if(rc != 0) {
		//todo
	}
	stmt->close();
}