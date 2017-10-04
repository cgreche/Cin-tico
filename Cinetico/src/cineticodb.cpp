
#include "cineticodb.h"
#include "entity/user/UserProfile.h"
#include "utils/crypter.h"
#include "database/sqlite/sqlitedatabase.h"

namespace cinetico {

const char *Table_UserProfile = "CREATE TABLE USER_PROFILE(id INTEGER PRIMARY KEY AUTOINCREMENT, login_name TEXT, password TEXT, creation_time INTEGER);";
const char *Table_Exercise = "CREATE TABLE EXERCISE(id INTEGER PRIMARY KEY, name TEXT, author TEXT, creation_date INTEGER, trackable_body_points INTEGER, is_public INTEGER, owner_id INTEGER REFERENCES USER_PROFILE(id));";
const char *Table_Action = "CREATE TABLE ACTION(id INTEGER PRIMARY KEY, exercise_id REFERENCES EXERCISE(id), type INTEGER NOT NULL, order_type INTEGER NOT NULL, name TEXT, min_time REAL, max_time REAL, body_point INTEGER NOT NULL, ref_point INTEGER NOT NULL, position_x REAL NOT NULL, position_y REAL NOT NULL, position_z REAL NOT NULL, orientation_x REAL, orientation_y REAL, orientation_z REAL);";
const char *Table_PositionAction = "CREATE TABLE POSITION_ACTION(id INTEGER PRIMARY KEY, action_id REFERENCES ACTION(id), min_hold_time REAL);";
const char *Table_MovementAction = "CREATE TABLE MOVEMENT_ACTION(id INTEGER PRIMARY KEY, action_id REFERENCES ACTION(id), movement_type INTEGER, min_speed REAL, max_speed REAL);";

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
			stmt = db.prepare(Table_PositionAction);
			if(!stmt->execute())
				int a = 1;
			stmt = db.prepare(Table_MovementAction);
			if(!stmt->execute())
				int a = 1;

			m_userProfileDAO = new UserProfileDAO(db);
			m_exerciseDAO = new ExerciseDAO(db);
			m_actionDAO = new ActionDAO(db);

			//criando default login
			UserProfile *adminProfile = m_userProfileDAO->getByLoginName("admin");
			if (!adminProfile) {
				adminProfile = new UserProfile("admin", Crypter::SimpleHash("admin"));
				m_userProfileDAO->create(*adminProfile);
				UserProfile *test = m_userProfileDAO->getByLoginName("admin");
				int a = 1;
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