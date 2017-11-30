
#include "Exercise.h"

namespace cinetico_core {

	Exercise::Exercise(unsigned long id) {
		m_id = id;
		m_state = Idle;
		m_body = NULL;
		m_currentActionIndex = -1;
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

		m_actions[m_currentActionIndex]->step(*m_body);
		if (m_actions[m_currentActionIndex]->state() == Action::Finished) {
			++m_currentActionIndex;
			if (m_currentActionIndex >= (int)m_actions.size()) {
				m_state = Finished;
				m_accuracy = calculateAccuracy();
			}
			else {
				m_actions[m_currentActionIndex]->start();
			}
		}

		return m_state;
	}

	void Exercise::cancel() {
		m_state = Canceled;
	}

	void Exercise::reset() {
		m_currentActionIndex = 0;
		//set all actions to Idle state
		for (unsigned int i = 0; i < m_actions.size(); ++i)
			m_actions[i]->stop();
		m_actions[0]->start();
		m_state = Running;
	}

}