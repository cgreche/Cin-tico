#ifndef __CINETICO_CORE_POSITIONACTION_H__
#define __CINETICO_CORE_POSITIONACTION_H__

#include "action.h"

namespace cinetico_core {

	class PositionAction : public Action {
		float m_minHoldTime;

		//state attributes
		float m_holdTime;

	public:
		PositionAction(Exercise &owner, int id = -1);
		virtual ActionResult avaliate(Body &body);

		void setMinHoldTime(float minHoldTime) { m_minHoldTime = minHoldTime; }
		float minHoldTime() const { return m_minHoldTime; }
	};

}

#endif