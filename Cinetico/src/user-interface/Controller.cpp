
#include "Controller.h"
#include "cinetico.h"
#include "cineticoui.h"

namespace cinetico {

	Controller::Controller(CineticoUI &cineticoUI)
		: m_cineticoUI(cineticoUI)
		, m_cinetico(cineticoUI.cinetico())
		, m_dictionary(*m_cinetico.dictionary()) {
	}

	void Controller::onViewEnter(ViewParams *params) { }
	void Controller::onViewUpdate() { }
	void Controller::onViewTick() { }
	void Controller::onViewQuit() { }
}