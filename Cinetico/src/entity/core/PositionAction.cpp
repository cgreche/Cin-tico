
#include "PositionAction.h"
#include "Exercise.h"

namespace cinetico_core {

	PositionAction::PositionAction(Exercise &owner, int id)
		: Action(Action::Position, owner, id) {
		m_minHoldTime = 0.5f;
		m_holdTime = 0.f;
	}

	Action::ActionResult PositionAction::avaliate(Body &body) {
		BodyPoint *bodyPoint = body.bodyPoint(m_bodyPoint);

		float comparingRadius = 0.3f;

		Vector3 diff = m_finalPosition - bodyPoint->position();
		float distanceSquared = diff.dotProduct(diff);

		float radiusSquared = comparingRadius*comparingRadius;
		
		float percent = distanceSquared / radiusSquared;
		
		if (m_holdTime >= m_minHoldTime) {
			if (percent < 0.5f)
				return Missed;
			if (percent < 0.8f)
				return Bad;
			if (percent < 0.9f)
				return Good;
			return Excellent;
		}

		return Missed;
	}

}