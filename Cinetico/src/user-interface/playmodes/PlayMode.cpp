
#include "PlayMode.h"
#include "cinetico.h"
#include "cineticoui.h"

namespace cinetico {

	PlayMode::PlayMode(Cinetico &cinetico, PlayModeID playMode)
		: m_cinetico(cinetico)
		, m_dictionary(*m_cinetico.dictionary()) {
		m_playingMode = playMode;
		m_renderEngine = cinetico.cineticoUI()->renderEngine();
		m_renderEngineHelper = cinetico.cineticoUI()->renderEngineHelper();
		setup();
	}

	PlayMode::~PlayMode() {
		cleanUp();
	}

}