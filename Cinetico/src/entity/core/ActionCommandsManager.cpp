
#include "ActionCommandsManager.h"
#include "Body.h"

namespace cinetico_core {

	ActionCommandsManager::ActionCommandsManager() {
	}

	ActionCommandsManager::~ActionCommandsManager() {
		for (unsigned int i = 0; i < m_commands.size(); ++i) {
			delete m_commands[i];
		}
	}

	void ActionCommandsManager::step(uilib::s64 curTime) {
		unsigned int i;
		if (!m_body)
			return;
		for (i = 0; i < m_trackedBps.size(); ++i) {
			ActionCommand *action = m_trackedBps[i].update(curTime);
			if (action) {
				m_commands.push_back(action);
				if (m_commands.size() > 10) {
					for (unsigned int i = 0; i < 5; ++i)
						delete m_commands[i];
					m_commands.erase(m_commands.begin(), m_commands.begin() + 5);
				}
			}
		}
	}

	void ActionCommandsManager::setBody(Body *body) {
		m_body = body;
		m_trackedBps.push_back(BodyPointState(*body->bodyPoint(BodyPoint::RightPalm), 0));
	}

}