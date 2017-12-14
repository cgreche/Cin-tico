
#include "database/database.h"
#include "ExerciseDAO.h"

namespace cinetico {

	ExerciseDAO::ExerciseDAO(Database &db)
		: m_db(db) {
	}

	void ExerciseDAO::create(Exercise &exercise, UserProfile *user) {
		const char *sql = "INSERT INTO EXERCISE(name,author,creation_date,trackable_body_points,is_public,owner_id) VALUES(?,?,?,?,?,?);";

		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, exercise.name()); //safer to use the created user properties
		stmt->bind(2, exercise.author());
		stmt->bind(3, 0);
		stmt->bind(4, (int)exercise.trackableBodyPoints());
		stmt->bind(5, exercise.isPublic() != false);
		if (user)
			stmt->bind(6, user->id());
		else
			stmt->bindNull(6);
		int rc = stmt->execute();
		stmt->close();
	}


	void ExerciseDAO::update(Exercise &exercise) {
		const char *sql = "UPDATE EXERCISE SET name = ?, author = ?, trackable_body_points = ?, is_public = ? WHERE id = ?;";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, exercise.name());
		stmt->bind(2, exercise.author());
		stmt->bind(3, (int)exercise.trackableBodyPoints());
		stmt->bind(4, exercise.isPublic());
		stmt->bind(5, (int)exercise.id());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo (future): retrieve proper error
		}
		stmt->close();
	}

	Exercise* ExerciseDAO::getUserExerciseByName(const char *name, UserProfile *user) {
		const char *sql = "SELECT * FROM EXERCISE WHERE owner_id = ? AND name = ?;";
		SQLStatement *stmt;
		std::vector<Exercise *> exerciseList;
		stmt = m_db.prepare(sql);
		stmt->bind(1, user->id());
		stmt->bind(2, name);

		Exercise *exercise = NULL;
		ResultSet *rs = stmt->query();
		if (rs) {
			while (rs->next()) {
				exercise = new Exercise(rs->getInt(0));
				exercise->setName(rs->getString(1).c_str());
				exercise->setAuthor(rs->getString(2).c_str());
				exercise->setTrackableBodyPoints(rs->getInt(4));
				exercise->setPublic(rs->getInt(5) != 0);
			}
			rs->close();
		}
		stmt->close();
		return exercise;
	}

	std::vector<Exercise *> ExerciseDAO::getAllExercisesByUserProfile(UserProfile *user) {
		const char *sql = "SELECT * FROM EXERCISE WHERE owner_id = ? OR is_public = 1;";
		SQLStatement *stmt;
		std::vector<Exercise *> exerciseList;
		stmt = m_db.prepare(sql);
		if (user)
			stmt->bind(1, user->id());
		else
			stmt->bindNull(1);

		ResultSet *rs = stmt->query();
		if (rs) {
			while (rs->next()) {
				Exercise *exercise = new Exercise(rs->getInt(0));
				exercise->setName(rs->getString(1).c_str());
				exercise->setAuthor(rs->getString(2).c_str());
				exercise->setTrackableBodyPoints(rs->getInt(4));
				exercise->setPublic(rs->getInt(5) != 0);
				exerciseList.push_back(exercise);
			}
			rs->close();
		}
		stmt->close();
		return exerciseList;
	}

	std::vector<Exercise *> ExerciseDAO::getAllExercises() {
		const char *sql = "SELECT * FROM EXERCISE;";
		SQLStatement *stmt;
		std::vector<Exercise *> exerciseList;
		stmt = m_db.prepare(sql);
		ResultSet *rs = stmt->query();
		if (rs) {
			while (rs->next()) {
				Exercise *exercise = new Exercise(rs->getInt(0));
				exercise->setName(rs->getString(1).c_str());
				exercise->setAuthor(rs->getString(2).c_str());
				exercise->setTrackableBodyPoints(rs->getInt(4));
				exercise->setPublic(rs->getInt(5) != 0);
				exerciseList.push_back(exercise);
			}
			rs->close();
		}
		stmt->close();
		return exerciseList;
	}

	void ExerciseDAO::exclude(Exercise &exercise) {
		SQLStatement *stmt;
		//Delete All Movement Gestures
		const char *sql = "DELETE FROM MOVEMENT_GESTURE WHERE simple_gesture_id IN (SELECT SIMPLE_GESTURE.id FROM SIMPLE_GESTURE, ACTION WHERE action_id=ACTION.id AND ACTION.exercide_id = ?);";
		stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		stmt->execute();
		//Delete All Simple Gestures
		sql = "DELETE FROM SIMPLE_GESTURE WHERE action_id IN (SELECT id FROM ACTION WHERE exercise_id = ?);";
		stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		stmt->execute();
		//Delete All Actions
		sql = "DELETE FROM ACTION WHERE exercise_id = ?;";
		stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		stmt->execute();
		//Delete the exercise
		sql = "DELETE FROM EXERCISE WHERE id = ?;";
		stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		stmt->execute();
		stmt->close();
	}

}