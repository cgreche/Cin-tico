
#include "PositionAction.h"
#include "Exercise.h"
#include <uilib/lib/time.h>

namespace cinetico_core {

	PositionAction::PositionAction(Exercise &owner, int id)
		: Action(Action::Position, owner, id) {
		m_minHoldTime = 0.5f;
		m_holdStartTime = 0.f;
	}

	Action::ActionResult PositionAction::avaliate(Body &body) {
		BodyPoint *bodyPoint = body.bodyPoint(m_bodyPoint);

		//todo: handle refPoint Any or LastPosition
		float gap = 0.2f;
		m_minHoldTime = 0.5f;
		Vector3 position = bodyPoint->position();

		Vector3 diff = m_lastPosition - position;

		float curTime = (float)uilib::OSTime::ticks();
		float tps = (float)uilib::OSTime::ticksPerSecond();
		float holdTime;
		if (sqrtf(diff.dotProduct(diff)) >= 0.1) //dotProduct here will square and sum
			m_holdStartTime = curTime;

		holdTime = (curTime - m_holdStartTime) / tps;

		float accuracyX = 0.f;
		float accuracyY = 0.f;
		float accuracyZ = 0.f;
		float weightX = m_refPointX >= 0 ? 1.f : 0.f;
		float weightY = m_refPointY >= 0 ? 1.f : 0.f;
		float weightZ = m_refPointZ >= 0 ? 1.f : 0.f;
		if (m_refPointX >= 0) {
			Vector3 refPos = body.bodyPoint((BodyPoint::BodyPart)m_refPointX)->position();
			float finalPos = m_finalPosition.x() + refPos.x();
			float diff = position.x() - finalPos;
			accuracyX = 100.f - fabsf(diff)*100.f / gap;
			if (accuracyX < 0.f)
				accuracyX = 0.f;
		}
		
		if (m_refPointY >= 0) {
			Vector3 refPos = body.bodyPoint((BodyPoint::BodyPart)m_refPointY)->position();
			float finalPos = m_finalPosition.y() + refPos.y();
			float diff = position.y() - finalPos;
			accuracyY = 100.f - fabsf(diff)*100.f / gap;
			if (accuracyY < 0.f)
				accuracyY = 0.f;
			weightY = 1.f;
		}
		
		if (m_refPointZ >= 0) {
			Vector3 refPos = body.bodyPoint((BodyPoint::BodyPart)m_refPointZ)->position();
			float finalPos = m_finalPosition.z() + refPos.z();
			float diff = position.z() - finalPos;
			accuracyZ = 100.f - fabsf(diff)*100.f / gap;
			if (accuracyZ < 0.f)
				accuracyZ = 0.f;
			weightZ = 1.f;
		}
		
		float accuracy = (accuracyX + accuracyY + accuracyZ) / (weightX + weightY + weightZ);
		m_accuracy = accuracy;
		if (holdTime >= m_minHoldTime) {
			if (accuracy < 50.f)
				return Missed;
			if (accuracy < 80.f)
				return Bad;
			if (accuracy < 90.f)
				return Good;
			return Excellent;
		}

		m_lastPosition = position;

		return Missed;
	}

}