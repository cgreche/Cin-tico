
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
		//Point we must position in the action
		BodyPoint *actionBodyPoint = body.bodyPoint(m_bodyPoint);

		//todo: handle refPoint Any or LastPosition
		float gap = 0.5f;
		m_minHoldTime = 0.5f;
		Vector3 actionPoint = actionBodyPoint->position();
		Vector3 targetPoint;

		float curTime = (float)uilib::OSTime::ticks();
		float tps = (float)uilib::OSTime::ticksPerSecond();
		float timeToHold;

		//Check if point position has changed "a lot"
		//If so, restart time as if it was a new position
		if (actionPoint.euclideanDistanceTo(m_lastPosition) >= 0.1)
			m_holdStartTime = curTime;

		timeToHold = (curTime - m_holdStartTime) / tps;

		BodyPoint *ref = body.bodyPoint((BodyPoint::BodyPart)m_refPoint);
		Vector3 refPos = ref->position();
		Vector3 refOrientation = ref->orientation();
		Vector3 refOrientationVec(0,0,-1);
		//refOrientationVec.setX(-std::sin(refOrientation.y())*std::cos(refOrientation.x()));
		//refOrientationVec.setY(std::sin(refOrientation.x()));
		//refOrientationVec.setZ(std::cos(refOrientation.y())*std::cos(refOrientation.x()));
		Vector3 frontPos = refOrientationVec;
		Vector3 rightPos = crossProduct(frontPos, Vector3(0, 1, 0));
		Vector3 upPos = crossProduct(rightPos,frontPos);

		int op = m_operation & 255;
		int posFlags = (m_operation >> 8) & 7;
		if (posFlags == 0)
			return Missed;

		targetPoint = refPos;
		if (posFlags & 1) {
			targetPoint += rightPos*m_finalPosition.x();
		}
		
		if (posFlags & 2) {
			targetPoint += upPos*m_finalPosition.y();
		}

		if (posFlags & 4) {
			targetPoint += frontPos*m_finalPosition.z();
		}
		
		float accuracy = 0.f;
		if (op == Action::InFront) {
			if (actionPoint.z() > targetPoint.z())
				accuracy = 100.f;
			else {
				accuracy = 1 - actionPoint.euclideanDistanceTo(targetPoint) / gap;
			}
		}
		else if (op == Action::Behind) {
			if (actionPoint.z() < targetPoint.z())
				accuracy = 100.f;
			else {
				accuracy = 1 - actionPoint.euclideanDistanceTo(targetPoint) / gap;
			}
		}
		else if (op == Action::ToRight) {
			if (actionPoint.x() > targetPoint.x())
				accuracy = 100.f;
			else {
				accuracy = 1 - actionPoint.euclideanDistanceTo(targetPoint) / gap;
			}
		}
		else if (op == Action::ToLeft) {
			if (actionPoint.x() > targetPoint.x())
				accuracy = 100.f;
			else {
				accuracy = 1 - actionPoint.euclideanDistanceTo(targetPoint) / gap;
			}
		}
		else if (op == Action::Above) {
			if (actionPoint.y() > targetPoint.y())
				accuracy = 100.f;
			else {
				accuracy = 1 - actionPoint.euclideanDistanceTo(targetPoint) / gap;
			}
		}
		else if (op == Action::Below) {
			if (actionPoint.y() < targetPoint.y())
				accuracy = 100.f;
			else {
				accuracy = 1 - actionPoint.euclideanDistanceTo(targetPoint) / gap;
			}
		}
		else if (op == Action::FixedPosition) {
			float accuracy = 1 - actionPoint.euclideanDistanceTo(targetPoint) / gap;
			accuracy *= 100.f;
		}
		else {
			return Missed;
		}

		if (accuracy < 0)
			accuracy = 0.f;
		if (accuracy > 100.f)
			accuracy = 100.f;

		m_accuracy = accuracy;
		if (timeToHold >= m_minHoldTime) {
			if (accuracy < 50.f)
				return Missed;
			m_state = Finished;
			if (accuracy < 80.f)
				return Bad;
			if (accuracy < 90.f)
				return Good;
			return Excellent;
		}

		m_lastPosition = actionPoint;
		return Missed;
	}

}