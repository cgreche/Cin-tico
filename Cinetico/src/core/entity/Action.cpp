#include "Action.h"
#include "SimpleGesture.h"

namespace cinetico_core {

	Action::Action(Exercise &owner, int id)
		: m_owner(owner), m_id(id) {
		m_minExecutionTime = m_maxExecutionTime = 0.f;
		m_state = Idle;
		m_result = Missed;
		m_accuracy = 0.f;
	}

	Action::~Action() {
		for (unsigned int i = 0; i < m_gestures.size(); ++i) {
			delete m_gestures[i];
		}
	}

	int Action::addGesture(SimpleGesture *gesture) {
		int id = m_gestures.size();
		m_gestures.push_back(gesture);
		m_gestureResults.push_back(0);
		return id;
	}

	void Action::start() {
		m_state = Running;
		m_result = Missed;
		m_accuracy = 0.f;
		for (unsigned int i = 0; i < m_gestureResults.size(); ++i)
			m_gestureResults[i] = 0;
	}

	void Action::stop() {
		m_state = Idle;
		m_accuracy = 0.f;
		for (unsigned int i = 0; i < m_gestureResults.size(); ++i)
			m_gestureResults[i] = 0;
	}

	bool Action::evaluate() {
		float accuracy = 0.f;
		bool result = true;
		for (unsigned int i = 0; i < m_gestureResults.size(); ++i) {
			//result 0 = not realized
			//result 1 = success
			//result 2 = incorrect
			if (m_gestureResults[i] == 1)
				accuracy += 1.f;
			else if (m_gestureResults[i] == 0)
				result = false;
		}

		m_accuracy = accuracy / m_gestures.size();
		m_accuracy *= 100.f;
		if (result) {
			if (accuracy < 0.5f)
				m_result = Missed;
			else if (accuracy < 0.75f)
				m_result = Bad;
			else if (accuracy < 0.95f)
				m_result = Good;
			else
				m_result = Excellent;

		}
		return result;
	}

	void Action::step(Body &body) {
		bool result = evaluate();
		if (result)
			m_state = Finished;
	}

}