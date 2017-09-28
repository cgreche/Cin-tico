
#ifndef __CINETICO_DB_H__
#define __CINETICO_DB_H__

#include "database/database.h"
#include "entity/dao/UserProfileDAO.h"
#include "entity/dao/ExerciseDAO.h"

namespace cinetico {

	using namespace cinetico_core;

	class Cinetico;

	//DB management module
	class CineticoDB {
		Cinetico &m_application;
		Database *m_db;

		UserProfileDAO *m_userProfileDAO;
		ExerciseDAO *m_exerciseDAO;

		void setup();
		void cleanUp();

	public:
		CineticoDB(Cinetico &cinetico);
		~CineticoDB();

		Database *db() const { return m_db; }
		UserProfileDAO *userProfileDAO() const { return m_userProfileDAO; }
		ExerciseDAO *exerciseDAO() const { return m_exerciseDAO; }
	};

}

#endif