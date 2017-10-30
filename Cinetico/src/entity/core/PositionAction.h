#ifndef __CINETICO_CORE_POSITIONACTION_H__
#define __CINETICO_CORE_POSITIONACTION_H__

#include "action.h"

namespace cinetico_core {

	class PositionAction : public Action {
		float m_minHoldTime;

		//state attributes
		Vector3 m_lastPosition;
		float m_holdStartTime;

	public:
		PositionAction(Exercise &owner, int id = -1);
		virtual ActionResult avaliate(Body &body);

		void setMinHoldTime(float minHoldTime) { 
			if (minHoldTime < 0.5f)
				minHoldTime = 0.5f;
			m_minHoldTime = minHoldTime;
		}

		float minHoldTime() const { return m_minHoldTime; }
	};

}

#endif