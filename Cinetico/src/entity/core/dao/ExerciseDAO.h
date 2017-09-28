#ifndef __CINETICO_EXERCISEDAO_H__
#define __CINETICO_EXERCISEDAO_H__

#include <vector>
#include <string>

#include "entity/core/Exercise.h"

class Database;

namespace cinetico_core {

	class ExerciseDAO
	{
		Database &m_db;

	public:
		ExerciseDAO(Database &db);

		void save(Exercise &exercise);
		void update(Exercise &exercise);
		std::vector<Exercise *> getAllExercises();
		void exclude(Exercise &exercise);

	};

}

#endif