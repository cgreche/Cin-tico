
#include "Exercise.h"

namespace cinetico_core {

	Exercise::Exercise(unsigned long id) {
		m_id = id;
	}

	Exercise::~Exercise() {
		for (unsigned int i = 0; i < m_actions.size(); ++i)
			delete m_actions[i];
	}

	void Exercise::insertAction(Action *action) {
		for (unsigned int i = 0; i < m_actions.size(); ++i)
			if (m_actions[i] == action)
				return;
		m_actions.push_back(action);
	}

	void Exercise::setActionList(std::vector<Action*> actionList) {
		m_actions = actionList;
	}

}