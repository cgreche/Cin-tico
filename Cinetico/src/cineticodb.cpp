
#include "cineticodb.h"
#include "entity/UserProfile.h"
#include "utils/crypter.h"
#include "database/sqlite/sqlitedatabase.h"
#include <cassert>

namespace cinetico {

const char *Table_UserProfile = "CREATE TABLE USER_PROFILE(id INTEGER PRIMARY KEY AUTOINCREMENT, login_name TEXT, password TEXT, user_name TEXT, creation_date INTEGER NOT NULL, activated INTEGER NOT NULL DEFAULT 1);";
const char *Table_Exercise = "CREATE TABLE EXERCISE(id INTEGER PRIMARY KEY, name TEXT, author TEXT, creation_date INTEGER, trackable_body_points INTEGER, is_public INTEGER, owner_id INTEGER REFERENCES USER_PROFILE(id));";
const char *Table_Action = "CREATE TABLE ACTION(id INTEGER PRIMARY KEY, exercise_id REFERENCES EXERCISE(id), name TEXT, min_execution_time REAL, max_execution_time REAL, time_to_hold REAL);";
const char *Table_SimpleGesture = "CREATE TABLE SIMPLE_GESTURE(id INTEGER PRIMARY KEY, action_id REFERENCES ACTION(id), transition_type INTEGER NOT NULL, body_point INTEGER NOT NULL, ref_point INTEGER NOT NULL, operation INTEGER NOT NULL, value_x REAL, value_y REAL, value_z REAL);";
const char *Table_MovementGesture = "CREATE TABLE MOVEMENT_GESTURE(simple_gesture_id PRIMARY KEY REFERENCES SIMPLE_GESTURE(id), min_speed REAL, max_speed REAL);";
const char *Table_MovementGesture_ExtraPoints = "CREATE TABLE MOVEMENT_GESTURE_EXTRA_POINTS(simple_gesture_id PRIMARY KEY REFERENCES SIMPLE_GESTURE(id), min_speed REAL, max_speed REAL);";
const char *Table_GeneralSettings = "CREATE TABLE GENERAL_SETTINGS(pos_dist_threshold REAL, pos_min_holdtime REAL, adapter INTEGER, resolution_width INTEGER, resolution_height INTEGER, refresh_rate INTEGER, fullscreen INTEGER, antialiasing INTEGER);";

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
		if (m_db)
			delete m_db;
	}

	void CineticoDB::setup() {
		m_db = DatabaseFactory::createDB(CinetiCoDefs::SGBDAlias, CinetiCoDefs::DatabaseFilename);
		Database &db = *m_db;
		db.open();
		if (db.isOpen()) {
			SQLStatement *stmt = db.prepare(Table_UserProfile);
			stmt->execute();
			stmt = db.prepare(Table_Exercise);
			stmt->execute();
			stmt = db.prepare(Table_Action);
			stmt->execute();
			stmt = db.prepare(Table_SimpleGesture);
			stmt->execute();
			stmt = db.prepare(Table_MovementGesture);
			stmt->execute();
			stmt = db.prepare(Table_MovementGesture_ExtraPoints);
			stmt->execute();
			stmt = db.prepare(Table_GeneralSettings);
			stmt->execute();

			m_userProfileDAO = new UserProfileDAO(db);
			m_exerciseDAO = new ExerciseDAO(db);
			m_actionDAO = new ActionDAO(db);
			m_generalSettingsDAO = new GeneralSettingsDAO(db);

			//criando default login
			UserProfile *adminProfile = m_userProfileDAO->getByLoginName("admin");
			if (!adminProfile) {
				adminProfile = new UserProfile("admin", Crypter::SimpleHash("admin"),"");
				m_userProfileDAO->create(*adminProfile);
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