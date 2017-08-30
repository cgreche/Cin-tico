
#include "database/database.h"

#include "entity/user/UserProfile.h"
#include "entity/user/dao/UserProfileDAO.h"

Database *g_db;

UserProfileDAO *userDAO;

void setupDB() {
	g_db = new Database("test.db");
	Database &db = *g_db;
	db.open();
	if(db.isOpen()) {
		const char *createUserProfile = "CREATE TABLE USER_PROFILE(login_name TEXT, password TEXT, creation_date INTEGER, last_login_date INTEGER);";
		SQLStatement *stmt1 = db.prepare(createUserProfile);

		UserProfileDAO *userDAO = new UserProfileDAO(db);
		//criando default login
		if(!userDAO->getByLoginName("admin")) {
			userDAO->create("admin", "admin");
		}

	}
}

void destroyDB() {
	if(userDAO)
		delete userDAO;
	g_db->close();
	delete g_db;
}