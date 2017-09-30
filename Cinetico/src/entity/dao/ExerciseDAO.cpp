
#include "database/database.h"
#include "ExerciseDAO.h"

namespace cinetico {

	ExerciseDAO::ExerciseDAO(Database &db)
		: m_db(db)
	{

	}

	void ExerciseDAO::save(Exercise &exercise) {
		const char *sql = "INSERT INTO EXERCISE(name,author,creation_date,trackable_body_points,is_public) VALUES(?,?,?,?,?);";

		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, exercise.name()); //safer to use the created user properties
		stmt->bind(2, exercise.author());
		stmt->bind(3, 0);
		stmt->bind(4, (int)exercise.trackableBodyPoints());
		stmt->bind(5, exercise.isPublic() != false);
		int rc = stmt->execute();
		if (rc != 0) {
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
		stmt->bind(3, exercise.trackableBodyPoints());
		stmt->bind(4, exercise.isPublic());
		stmt->bind(5, exercise.id());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo
		}
		stmt->close();
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
		const char *sql = "DELETE FROM EXERCISE WHERE id = ?;";
		SQLStatement *stmt;
		stmt = m_db.prepare(sql);
		stmt->bind(1, exercise.id());
		int rc = stmt->execute();
		if (rc != 0) {
			//todo
		}
		stmt->close();
	}

}