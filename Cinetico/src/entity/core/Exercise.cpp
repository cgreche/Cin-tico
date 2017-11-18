
#include "Exercise.h"

namespace cinetico_core {

	Exercise::Exercise(unsigned long id) {
		m_id = id;
		m_state = Idle;
		m_body = NULL;
		m_currentActionIndex = -1;
		m_currentActionIndexesIndex = 0;
		m_accuracy = 0.f;
	}

	Exercise::~Exercise() {
		for (unsigned int i = 0; i < m_actions.size(); ++i)
			delete m_actions[i];
	}

	float Exercise::calculateAccuracy() {
		int totalActions = m_actions.size();
		float accuracy = 0.f;
		if (totalActions > 0) {
			for (int i = 0; i < totalActions; ++i) {
				accuracy += m_actions[i]->accuracy();
			}
			accuracy /= totalActions;
		}
		return accuracy;
	}

	void Exercise::setActionList(std::vector<Action*> actionList) {
		m_actions = actionList;
		if (!m_actionIndexList.empty())
			m_actionIndexList.clear();
		for (int i = 0; i < actionList.size(); ++i) {
			if(actionList[i]->order() == Action::NewAction)
				m_actionIndexList.push_back(i);
		}
	}

	void Exercise::start(Body &body) {
		if (m_actions.size() == 0) {
			m_state = Finished;
			return;
		}

		m_body = &body;
		reset();
	}

	Exercise::ExerciseState Exercise::step() {
		if (m_state != Running)
			return m_state;

		bool actionFinished = true;
		for (int i = 0; i < m_currentActionList.size(); ++i) {
			Action *action = m_currentActionList[i];
			action->step(*m_body);
			if (action->state() != Action::Finished) {
				actionFinished = false;
				break;
			}
		}

		if (actionFinished) {
			m_currentActionIndexesIndex++;
			if (m_currentActionIndexesIndex >= m_actionIndexList.size()) {
				m_state = Finished;
				m_accuracy = calculateAccuracy();
			}
			else {
				getNextActions();
				for (int i = 0; i < m_currentActionList.size(); ++i) {
					Action *action = m_currentActionList[i];
					action->start();
				}
			}
		}

		return m_state;
	}

	void Exercise::cancel() {
		m_state = Canceled;
	}

	void Exercise::reset() {
		m_currentActionIndex = 0;
		m_currentActionIndexesIndex = 0;
		getNextActions();
		//set all actions to Idle state
		for (int i = 0; i < m_actions.size(); ++i)
			m_actions[i]->stop();
		for (int i = 0; i < m_currentActionList.size(); ++i)
			m_actions[i]->start();
		m_state = Running;
	}

	int Exercise::getNextActions() {
		if (!m_currentActionList.empty())
			m_currentActionList.clear();

		int index = m_currentActionIndexesIndex;
		if (index >= m_actions.size())
			return 0;
		m_currentActionList.push_back(m_actions[m_actionIndexList[index]]);
		++index;
		for (; index < m_actions.size(); ++index) {
			Action *action = m_actions[index];
			if (action->order() == Action::NewAction)
				return 0;
			m_currentActionList.push_back(action);
		}

		return 0;
	}

}