#ifndef __CINETICO_GENERAL_SETTINGS_DAO_H__
#define __CINETICO_GENERAL_SETTINGS_DAO_H__

#include "entity/GeneralSettings.h"

namespace cinetico {

	class GeneralSettingsDAO
	{
		Database &m_db;
		
	public:
		GeneralSettingsDAO(Database &db);
		void save(GeneralSettings &settings);
		void get(GeneralSettings &settings);
	};

}

#endif