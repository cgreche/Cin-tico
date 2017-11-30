#ifndef __CINETICO_EXERCISEDAO_H__
#define __CINETICO_EXERCISEDAO_H__

#include <vector>
#include <string>

#include "entity/UserProfile.h"
#include "core/entity/Exercise.h"

class Database;

namespace cinetico {

	using namespace cinetico_core;

	class ExerciseDAO
	{
		Database &m_db;

	public:
		ExerciseDAO(Database &db);

		void create(Exercise &exercise, UserProfile *owner);
		void update(Exercise &exercise);
		Exercise* getUserExerciseByName(const char *name, UserProfile *user);
		std::vector<Exercise *> getAllExercises();
		std::vector<Exercise *> getAllExercisesByUserProfile(UserProfile *user);
		void exclude(Exercise &exercise);
	};

}

#endif