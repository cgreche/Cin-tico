
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

		float comparingRadius = 0.03f;
		float threshold = 0.3f;

		Vector3 refPos;
		if (m_refPoint >= 0) {
			refPos = body.bodyPoint((BodyPoint::BodyPart)m_refPoint)->position();
		}

		Vector3 finalPosition = m_finalPosition + refPos;

		Vector3 diff = bodyPoint->position() - refPos;

		float accuracyX = 100.f;
		float accuracyY = 100.f;
		float accuracyZ = 100.f;
		if (m_finalPosition.x() != 0.f) {
			accuracyX = finalPosition.x() / refPos.x() * 100.f;
		}
		
		if (m_finalPosition.y() != 0.f) {
			accuracyY = finalPosition.y() / refPos.y() * 100.f;
		}
		
		if (m_finalPosition.z() != 0.f) {
			accuracyZ = finalPosition.z() / refPos.z() * 100.f;
		}

		float distanceSquared = diff.dotProduct(diff);

		float radiusSquared = comparingRadius*comparingRadius;
		
		float accuracy = (accuracyX + accuracyY + accuracyZ) / 3.f;
		m_accuracy = accuracy;
		if (m_holdTime >= m_minHoldTime) {
			if (accuracy < 0.5f)
				return Missed;
			if (accuracy < 0.8f)
				return Bad;
			if (accuracy < 0.9f)
				return Good;
			return Excellent;
		}

		return Missed;
	}

}