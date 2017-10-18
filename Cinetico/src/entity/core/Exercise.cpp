
#include "Exercise.h"

namespace cinetico_core {

	Exercise::Exercise(unsigned long id) {
		m_id = id;
		m_state = Idle;
		m_body = NULL;
		m_currentActionIndex = -1;
	}

	Exercise::~Exercise() {
		for (unsigned int i = 0; i < m_actions.size(); ++i)
			delete m_actions[i];
	}

	void Exercise::setActionList(std::vector<Action*> actionList) {
		m_actions = actionList;
	}

	void Exercise::start(Body &body) {
		if (m_actions.size() == 0) {
			m_state = Finished;
			return;
		}

		m_body = &body;

		m_currentActionIndex = 0;
		m_state = Running;
	}

	Exercise::ExerciseState Exercise::step() {
		if (m_state != Running)
			return m_state;

		unsigned int index = m_currentActionIndex;
		Action *action = m_actions[m_currentActionIndex];
		action->step(*m_body);
		bool actionFinished = action->state() == Action::Finished;
		++index;
		for (; index < m_actions.size() && actionFinished; ++index) {
			action = m_actions[index];
			if (action->order() != Action::SameLastAction)
				break;
			action->step(*m_body);
			if (action->state() != Action::Finished)
				actionFinished = false;
		}

		if (actionFinished)
			index = getNextActionsIndex();

		if (index >= m_actions.size())
			m_state = Finished;

		m_currentActionIndex = index;
		return m_state;
	}

	void Exercise::cancel() {
		m_state = Canceled;
	}

	void Exercise::reset() {
		m_currentActionIndex = 0;
		m_state = Running;
	}

	int Exercise::getNextActionsIndex() {
		int index = m_currentActionIndex+1;
		for (; index < m_actions.size(); ++index) {
			Action *action = m_actions[index];
			if (action->order() == Action::NewAction)
				return index;
		}

		return index;
	}

}