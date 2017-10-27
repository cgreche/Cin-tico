
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
		if (rc == 0) {
			const char *sqlAction = "INSERT INTO ACTION(exercise_id,tag,order) VALUES(?,?)";
			const char *sqlActionPosition = "INSERT INTO ACTION_POSITION(action_id) VALUES(?)";
			const char *sqlActionMovement = "INSERT INTO ACTION_MOVEMENT(action_id) VALUES(?)";
			SQLStatement *stmtAction = m_db.prepare(sqlAction);
			SQLStatement *stmtActionPosition = m_db.prepare(sqlActionPosition);
			SQLStatement *stmtActionMovement = m_db.prepare(sqlActionMovement);
			for (unsigned int i = 0; i < exercise.actionCount(); ++i) {
				Action *action = exercise.action(i);
				stmtAction->bind(1, (int)exercise.id());
				stmtAction->bind(2, action->name().c_str());
				stmtAction->bind(3,(int)i);
				stmt->execute();
				if (action->type() == Action::Position) {
					stmtActionPosition->bind(1, action->id());
					stmtActionPosition->execute();
				}
				else if (action->type() == Action::Movement) {
					stmtActionMovement->bind(1, action->id());
					stmtActionMovement->execute();
				}
			}
			stmtAction->close();
			stmtActionPosition->close();
			stmtActionMovement->close();
		}
		else {
			//todo
		}
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
			//todo
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
		const char *sql = "DELETE FROM ACTION WHERE exercise_id = ?";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		stmt->execute();

		sql = "DELETE FROM EXERCISE WHERE id = ? ; ";
		stmt = m_db.prepare(sql);
		stmt->bind(1, (int)exercise.id());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo
		}
		stmt->close();
	}

}