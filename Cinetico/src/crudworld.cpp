
#include "database/database.h"

#include "entity/user/UserProfile.h"
#include "entity/user/dao/UserProfileDAO.h"

Database *g_db;

UserProfileDAO *userProfileDAO;

//todo: add attribute/column last_login_date to UserProfile

void setupDB() {
	g_db = new Database("test.db");
	Database &db = *g_db;
	db.open();
	if(db.isOpen()) {
		const char *createUserProfile = "CREATE TABLE USER_PROFILE(id INTEGER PRIMARY KEY, login_name TEXT, password TEXT, creation_date INTEGER);";
		SQLStatement *stmt1 = db.prepare(createUserProfile);

		userProfileDAO = new UserProfileDAO(db);
		//criando default login
		UserProfile *adminProfile = userProfileDAO->getByLoginName("admin");
		if(!adminProfile) {
			adminProfile = new UserProfile("admin", "admin");
			userProfileDAO->save(*adminProfile);
		}

		delete adminProfile;
	}
}

void destroyDB() {
	if(userProfileDAO)
		delete userProfileDAO;
	g_db->close();
	delete g_db;
}