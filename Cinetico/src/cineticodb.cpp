
#include "cineticodb.h"
#include "entity/user/UserProfile.h"
#include "utils/crypter.h"
#include "database/sqlite/sqlitedatabase.h"

namespace cinetico {

const char *Table_UserProfile = "CREATE TABLE USER_PROFILE(id INTEGER PRIMARY KEY, login_name TEXT, password TEXT, creation_date INTEGER);";
const char *Table_Exercise = "CREATE TABLE USER_PROFILE(id INTEGER PRIMARY KEY, author REFERENCES USER_PROFILE(login_name), exercise_name TEXT, creation_date INTEGER, identifiable_body_points INTEGER, public INTEGER);";
const char *Table_Action = "CREATE TABLE ACTION(id INTEGER PRIMARY KEY, exercise_id REFERENCES EXERCISE(id), type INTEGER NOT NULL, min_execution_time INTEGER, max_execution_time INTEGER);";
const char *Table_SpaceAction = "CREATE TABLE SPACE_ACTION(id INTEGER PRIMARY KEY, action_id REFERENCES ACTION(id), space_type_x INTEGER, space_type_y INTEGER, space_type_z INTEGER, position_x REAL, position_y REAL, position_z REAL);";
const char *Table_PositionAction = "CREATE TABLE POSITION_ACTION(id INTEGER PRIMARY KEY, space_action_id REFERENCES SPACE_ACTION(id), hold_time REAL);";
const char *Table_MovementAction = "CREATE TABLE MOVEMENT_ACTION(id INTEGER PRIMARY KEY, spacE_action_id REFERENCES SPACE_ACTION(id), movement_type INTEGER);";


	class CinetiCoDefs {
	public:
		static const char *SGBDAlias;
		static const char *DatabaseFilename;
	};

	const char *CinetiCoDefs::SGBDAlias = "SQLite";
	const char *CinetiCoDefs::DatabaseFilename = "demodb.db";

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

	CineticoDB::CineticoDB(Cinetico &cinetico)
		: m_application(cinetico) {
		setup();
	}

	CineticoDB::~CineticoDB() {

	}

	void CineticoDB::setup() {
		m_db = DatabaseFactory::createDB(CinetiCoDefs::SGBDAlias, CinetiCoDefs::DatabaseFilename);
		Database &db = *m_db;
		db.open();
		if (db.isOpen()) {
			SQLStatement *stmt = db.prepare(Table_UserProfile);
			if(!stmt->execute())
				int a = 1;
			stmt = db.prepare(Table_Exercise);
			if(!stmt->execute())
				int a = 1;
			stmt = db.prepare(Table_Action);
			if(!stmt->execute())
				int a = 1;
			stmt = db.prepare(Table_SpaceAction);
			if(!stmt->execute())
				int a = 1;
			stmt = db.prepare(Table_PositionAction);
			if(!stmt->execute())
				int a = 1;
			stmt = db.prepare(Table_MovementAction);
			if(!stmt->execute())
				int a = 1;

			m_userProfileDAO = new UserProfileDAO(db);
			//criando default login
			UserProfile *adminProfile = m_userProfileDAO->getByLoginName("admin");
			if (!adminProfile) {
				adminProfile = new UserProfile("admin", Crypter::SimpleHash("admin"));
				m_userProfileDAO->save(*adminProfile);
			}

			delete adminProfile;
		}
	}

	void CineticoDB::cleanUp() {
		if (m_userProfileDAO)
			delete m_userProfileDAO;
		m_db->close();
	}

}