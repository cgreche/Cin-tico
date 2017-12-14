
#include "database/database.h"
#include "UserProfileDAO.h"

namespace cinetico {

	UserProfileDAO::UserProfileDAO(Database &db)
		: m_db(db)
	{

	}

	void UserProfileDAO::create(UserProfile &user) {
		const char *sql = "INSERT INTO USER_PROFILE(login_name,password,user_name,creation_date) VALUES(?,?,?,?);";

		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, user.loginName()); //safer to use the created user properties
		stmt->bind(2, user.password());
		stmt->bind(3, user.name());
		stmt->bind(4, (int)user.creationDate());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo (future): retrieve proper error
		}
		stmt->close();
	}


	void UserProfileDAO::update(UserProfile &user) {
		const char *sql = "UPDATE USER_PROFILE SET login_name = ?, password = ?, user_name = ?;";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, user.loginName());
		stmt->bind(2, user.password());
		stmt->bind(3, user.name());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo (future): retrieve proper error
		}
		stmt->close();
	}

	std::vector<UserProfile *> UserProfileDAO::getAllUsers() {
		const char *sql = "SELECT * FROM USER_PROFILE WHERE activated = 1;";
		SQLStatement *stmt;
		std::vector<UserProfile *> userList;
		stmt = m_db.prepare(sql);
		ResultSet *rs = stmt->query();
		if (rs) {
			while (rs->next()) {
				UserProfile *user = new UserProfile(rs->getString(1), rs->getString(2), rs->getString(3), rs->getInt(4), rs->getInt(0));
				userList.push_back(user);
			}
			rs->close();
		}
		stmt->close();
		return userList;
	}

	UserProfile *UserProfileDAO::getByLoginName(const char *loginName) {
		const char *sql = "SELECT * FROM USER_PROFILE WHERE login_name = ? AND activated = 1;";
		UserProfile *user = NULL;
		SQLStatement *stmt = m_db.prepare(sql);
		stmt->bind(1, loginName);
		ResultSet *rs = stmt->query();
		if (rs) {
			if (rs->next())
				user = new UserProfile(rs->getString(1), rs->getString(2), rs->getString(3), rs->getInt(4), rs->getInt(0));
		}
		stmt->close();
		return user;
	}

	void UserProfileDAO::exclude(UserProfile &user) {
		const char *sql = "UPDATE USER_PROFILE SET activated = 0 WHERE login_name = ?;";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, user.loginName());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo (future): retrieve proper error
		}
		stmt->close();
	}

	void UserProfileDAO::exclude(const char *loginName) {
		const char *sql = "UPDATE USER_PROFILE SET activated = 0 WHERE login_name = ?;";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, loginName);
		int rc = stmt->execute();
		if (rc != 0) {
			//todo (future): retrieve proper error
		}
		stmt->close();
	}

}