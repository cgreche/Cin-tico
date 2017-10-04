#ifndef __CINETICO_ACTIONDAO_H__
#define __CINETICO_ACTIONDAO_H__

#include <vector>
#include <string>

#include "entity/core/Exercise.h"
#include "entity/core/Action.h"

class Database;

namespace cinetico {
	
	using namespace cinetico_core;

	class ActionDAO
	{
		Database &m_db;

	public:
		ActionDAO(Database &db);

		void create(Action &action);
		void update(Action &exercise);
		std::vector<Action *> getAllActionsByExercise(Exercise &exercise);
		void exclude(Action &exercise);
	};

}

#endif