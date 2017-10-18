
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

		float gap = 0.2f;

		Vector3 refPos;
		if (m_refPoint >= 0) {
			refPos = body.bodyPoint((BodyPoint::BodyPart)m_refPoint)->position();
		}

		Vector3 finalPosition = m_finalPosition + refPos;
		Vector3 diff = bodyPoint->position() - finalPosition;

		float accuracyX = 0.f;
		float accuracyY = 0.f;
		float accuracyZ = 0.f;
		float weightX = 0.f;
		float weightY = 0.f;
		float weightZ = 0.f;
		if (m_finalPosition.x() != 0.f) {
			accuracyX = 100.f - fabsf(diff.x())*100.f / gap;
			if (accuracyX < 0.f)
				accuracyX = 0.f;
			weightX = 1.f;
		}
		
		if (m_finalPosition.y() != 0.f) {
			accuracyY = 100.f - fabsf(diff.y())*100.f / gap;
			if (accuracyY < 0.f)
				accuracyY = 0.f;
			weightY = 1.f;
		}
		
		if (m_finalPosition.z() != 0.f) {
			accuracyZ = 100.f - fabsf(diff.z())*100.f / gap;
			if (accuracyZ < 0.f)
				accuracyZ = 0.f;
			weightZ = 1.f;
		}
		
		float accuracy = (accuracyX + accuracyY + accuracyZ) / (weightX + weightY + weightZ);
		m_accuracy = accuracy;
//		if (m_holdTime >= m_minHoldTime) {
			if (accuracy < 50.f)
				return Missed;
			if (accuracy < 80.f)
				return Bad;
			if (accuracy < 90.f)
				return Good;
			return Excellent;
//		}

		return Missed;
	}

}