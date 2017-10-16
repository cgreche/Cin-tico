
#include "Controller.h"
#include "cinetico.h"

namespace cinetico {

	Controller::Controller(Cinetico &cinetico)
		: m_cinetico(cinetico)
		, m_dictionary(*cinetico.dictionary()) {

	}

	void Controller::onViewEnter(ViewParams *params) { }
	void Controller::onViewUpdate() { }
	void Controller::onViewTick() { }
	void Controller::onViewQuit() { }
}