
#include "database/database.h"
#include "entity/user/UserProfile.h"
#include "entity/user/dao/UserProfileDAO.h"
#include "utils/crypter.h"
#include "database/sqlite/sqlitedatabase.h"

namespace cinetico {

	Database *g_db;

	UserProfileDAO *userProfileDAO;

	//todo: add attribute/column last_login_time to UserProfile

#define SGBD_ALIAS "SQLite"


	class DatabaseFactory
	{
	public:
		static Database *createDB(const char* sgbd, const char *dbName);
	};


	Database* DatabaseFactory::createDB(const char *sgbd, const char *dbName) {
		if (strcmp("SQLite", sgbd) == 0) {
			return new SQLiteDatabase(dbName);
		}

		throw "Unsupported SGBD";
	}

	void setupDB() {
		g_db = DatabaseFactory::createDB(SGBD_ALIAS, "demodb.db");
		Database &db = *g_db;
		db.open();
		if (db.isOpen()) {
			const char *createUserProfile = "CREATE TABLE USER_PROFILE(id INTEGER PRIMARY KEY, login_name TEXT, password TEXT, creation_date INTEGER);";
			SQLStatement *stmt1 = db.prepare(createUserProfile);
			stmt1->execute();

			userProfileDAO = new UserProfileDAO(db);
			//criando default login
			UserProfile *adminProfile = userProfileDAO->getByLoginName("admin");
			if (!adminProfile) {
				adminProfile = new UserProfile("admin", Crypter::SimpleHash("admin"));
				userProfileDAO->save(*adminProfile);
			}

			delete adminProfile;
		}
	}

	void destroyDB() {
		if (userProfileDAO)
			delete userProfileDAO;
		g_db->close();
	}

}