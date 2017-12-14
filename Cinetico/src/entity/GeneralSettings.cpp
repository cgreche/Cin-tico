
#include "GeneralSettings.h"

namespace cinetico {
	
	GeneralSettings::GeneralSettings() {
		m_posDistThreshold = 0.6f;
		m_posMinHoldtime = 0.25f;
		m_adapter = 0;
		m_resolutionWidth = 1024;
		m_resolutionHeight = 768;
		m_refreshRate = 60;
		m_fullscreen = true;
		m_antialiasing = true;
	}

}