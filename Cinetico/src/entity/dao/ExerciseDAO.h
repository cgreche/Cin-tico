#ifndef __CINETICO_EXERCISEDAO_H__
#define __CINETICO_EXERCISEDAO_H__

#include <vector>
#include <string>

#include "entity/user/UserProfile.h"
#include "entity/core/Exercise.h"

class Database;

namespace cinetico {

	using namespace cinetico_core;

	class ExerciseDAO
	{
		Database &m_db;

	public:
		ExerciseDAO(Database &db);

		void save(Exercise &exercise, UserProfile *owner);
		void update(Exercise &exercise);
		std::vector<Exercise *> getAllExercises();
		std::vector<Exercise *> getAllExercisesByUserProfile(UserProfile *user);
		void exclude(Exercise &exercise);
	};

}

#endif