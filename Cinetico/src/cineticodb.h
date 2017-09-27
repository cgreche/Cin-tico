
#ifndef __CINETICO_DB_H__
#define __CINETICO_DB_H__

#include "database/database.h"
#include "entity/user/dao/UserProfileDAO.h"

namespace cinetico {

	class Cinetico;

	//DB management module
	class CineticoDB {
		Cinetico &m_application;
		Database *m_db;

		UserProfileDAO *m_userProfileDAO;

		void setup();
		void cleanUp();

	public:
		CineticoDB(Cinetico &cinetico);
		~CineticoDB();

		Database *db() const { return m_db; }
		UserProfileDAO *userProfileDAO() const { return m_userProfileDAO; }
	};

}

#endif