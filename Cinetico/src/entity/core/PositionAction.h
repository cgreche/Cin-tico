#ifndef __CINETICO_CORE_POSITIONACTION_H__
#define __CINETICO_CORE_POSITIONACTION_H__

#include "action.h"

namespace cinetico_core {

	class PositionAction : public Action {
		float m_minHoldTime;

	public:
		PositionAction(Exercise &owner, int id = -1)
			: Action(Action::Position, owner, id) {
			m_minHoldTime = 0.f;
		}

		void setMinHoldTime(float minHoldTime) { m_minHoldTime = minHoldTime; }
		float minHoldTime() const { return m_minHoldTime; }
	};

}

#endif