
#include "database/database.h"
#include "GeneralSettingsDAO.h"

namespace cinetico {
	
	GeneralSettingsDAO::GeneralSettingsDAO(Database &db)
		: m_db(db) {
	}

	void GeneralSettingsDAO::save(GeneralSettings &settings) {
		const char *sql = "SELECT * FROM GENERAL_SETTINGS;";

		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		ResultSet *rs = stmt->query();
		bool update = false;
		if (rs->next()) {
			//registry already exists. Update.
			update = true;
		}
		rs->close();
		stmt->close();

		if (update) {
			sql = "UPDATE GENERAL_SETTINGS SET pos_dist_threshold=?, pos_min_holdtime = ?, adapter=?, resolution_width=?, resolution_height=?"
				", refresh_rate=?, fullscreen=?, antialiasing=?;";
			stmt = m_db.prepare(sql);
		}
		else {
			sql = "INSERT INTO GENERAL_SETTINGS(pos_dist_threshold, pos_min_holdtime, adapter, resolution_width, resolution_height"
				", refresh_rate, fullscreen, antialiasing)"
				" VALUES(?,?,?,?,?,?,?,?);";
			stmt = m_db.prepare(sql);
		}

		stmt->bind(1, settings.posDistThreshold());
		stmt->bind(2, settings.posMinHoldtime());
		stmt->bind(3, settings.adapter());
		stmt->bind(4, settings.resolutionWidth());
		stmt->bind(5, settings.resolutionHeight());
		stmt->bind(6, settings.refreshRate());
		stmt->bind(7, settings.fullscreen() ? 1 : 0);
		stmt->bind(8, settings.antialiasing() ? 1 : 0);
		int rc = stmt->execute();
		stmt->close();
	}

	void GeneralSettingsDAO::get(GeneralSettings &settings) {
		const char *sql = "SELECT * FROM GENERAL_SETTINGS;";

		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		ResultSet *rs = stmt->query();
		bool update = false;
		if (rs->next()) {
			settings.setPosDistThreshold(rs->getFloat(0));
			settings.setPosMinHoldtime(rs->getFloat(1));
			settings.setAdapter(rs->getInt(2));
			settings.setResolutionWidth(rs->getInt(3));
			settings.setResolutionHeight(rs->getInt(4));
			settings.setRefreshRate(rs->getInt(5));
			settings.setFullscreen(rs->getInt(6) == 1);
			settings.setAntialiasing(rs->getInt(7) == 1);
		}
		rs->close();
		stmt->close();
	}

}