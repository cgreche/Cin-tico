
#include "GestureCommandsManager.h"
#include "entity/Body.h"
#include "entity/SimpleGesture.h"

namespace cinetico_core {

	GestureCommandsManager::GestureCommandsManager() {
	}

	GestureCommandsManager::~GestureCommandsManager() {
		for (unsigned int i = 0; i < m_commands.size(); ++i) {
			delete m_commands[i];
		}
	}

	void GestureCommandsManager::reset() {
		if (!m_commands.empty())
			m_commands.clear();
	}

	void GestureCommandsManager::setBody(Body *body) {
		m_body = body;
		//todo: set proper tracked points
		//m_trackedBps.push_back(BodyPointState(*body->bodyPoint((BodyPoint::BodyPart)BodyPoint::RightPalm), 0));
		
		for (unsigned int i = 0; i < BodyPoint::BodyPartCount; ++i) {
			m_trackedBps.push_back(BodyPointState(*body->bodyPoint((BodyPoint::BodyPart)i), 0));
		}
		
	}

	void GestureCommandsManager::step(uilib::u64 curTime) {
		unsigned int i;
		m_curTime = curTime;
		if (!m_body)
			return;

		if (!m_commands.empty())
			m_commands.clear();

		for (unsigned int i = 0; i < BodyPoint::BodyPartCount; ++i) {
			ActionCommand *actionCommand = m_trackedBps[i].update(curTime);
			if (actionCommand)
				m_commands.push_back(actionCommand);
		}
		/*
		ActionCommand *actionCommand = m_trackedBps[0].update(curTime);
		if (actionCommand)
			m_commands.push_back(actionCommand);
		*/
	}



	std::vector<ActionCommand*> GestureCommandsManager::filterCommands(int transitionType, BodyPoint::BodyPart bp) {
		unsigned int i;
		std::vector<ActionCommand*> ret;
		for (i = 0; i < m_commands.size(); ++i) {
			if (transitionType == 0 && m_commands[i]->positionActionCommand()) {
				if ((m_commands[i]->initTime() == m_curTime || m_commands[i]->lastUpdateTime() == m_curTime)
					&& m_commands[i]->bodyPoint().bodyPart() == bp)
					ret.push_back(m_commands[i]);
			}
			else if (transitionType == 1 && m_commands[i]->movementActionCommand()) {
				if (m_commands[i]->endTime() == m_curTime && m_commands[i]->bodyPoint().bodyPart() == bp)
					ret.push_back(m_commands[i]);
			}
		}
		return ret;
	}

	bool GestureCommandsManager::meetConditions(SimpleGesture *gesture, ActionCommand *command, float distThreshold) {
		//todo: handle refPoint Any or LastPosition

		Vector3 actionPoint;
		if (command->movementActionCommand())
			actionPoint = command->movementActionCommand()->endPosition();
		else
			actionPoint = command->initPosition();
		Vector3 targetPoint;

		BodyPoint *ref = m_body->bodyPoint((BodyPoint::BodyPart)gesture->refPoint());
		Vector3 refPos = ref->position();
		Vector3 refOrientation = ref->orientation().toEuler();
		Vector3 refOrientationVec(0, 0, -1);
		//refOrientationVec.setX(-std::sin(refOrientation.y())*std::cos(refOrientation.x()));
		//refOrientationVec.setY(std::sin(refOrientation.x()));
		//refOrientationVec.setZ(std::cos(refOrientation.y())*std::cos(refOrientation.x()));
		Vector3 frontPos = refOrientationVec;
		Vector3 rightPos = crossProduct(frontPos, Vector3(0, 1, 0));
		Vector3 upPos = crossProduct(rightPos, frontPos);

		unsigned long operation = gesture->operation();
		int op = operation & 255;
		int posFlags = (operation >> 8) & 7;
		Vector3 finalPosition = gesture->value();
		if (posFlags == 0)
			return false;

		targetPoint = refPos;
		if (posFlags & 1) {
			targetPoint += rightPos*finalPosition.x();
		}

		if (posFlags & 2) {
			targetPoint += upPos*finalPosition.y();
		}

		if (posFlags & 4) {
			targetPoint += frontPos*finalPosition.z();
		}

		float accuracy = 0.f;

		//further to sensor: +z
		//closer to sensor: -z
		if (op == SimpleGesture::InFront) {
			return actionPoint.z() < targetPoint.z();
		}
		else if (op == SimpleGesture::Behind) {
			return actionPoint.z() > targetPoint.z();
		}
		else if (op == SimpleGesture::ToRight) {
			return actionPoint.x() > targetPoint.x();
		}
		else if (op == SimpleGesture::ToLeft) {
			return actionPoint.x() > targetPoint.x();
		}
		else if (op == SimpleGesture::Above) {
			return actionPoint.y() > targetPoint.y();
		}
		else if (op == SimpleGesture::Below) {
			return actionPoint.y() < targetPoint.y();
		}
		else if (op == SimpleGesture::FixedPosition) {
			return actionPoint.euclideanDistanceTo(targetPoint) <= distThreshold;
		}
		else if (op == SimpleGesture::AtSameBreadth) {
			return fabsf(actionPoint.x() - targetPoint.x()) <= distThreshold;
		}
		else if (op == SimpleGesture::AtSameHeight) {
			return fabsf(actionPoint.y() - targetPoint.y()) <= distThreshold;
		}
		else if (op == SimpleGesture::AtSameDepth) {
			return fabsf(actionPoint.z() - targetPoint.z()) <= distThreshold;
		}
		else if (op == SimpleGesture::FixedOrientation) {
			//todo
		}
		else if (op == SimpleGesture::OrientationLookingTo) {
			//todo
		}

		return false;
	}

	void GestureCommandsManager::checkConditions(Action &action, float distThreshold) {
		int gestureCount = action.gestureCount();

		for (int i = 0; i < gestureCount; ++i) {
			SimpleGesture *gesture = action.gesture(i);
			BodyPoint::BodyPart bp = gesture->bodyPoint();
			if (gesture->transitionType() == SimpleGesture::Free) {
				std::vector<ActionCommand*> actions = filterCommands(0, bp);
				if (!actions.empty()) {
					int gestureResult = meetConditions(gesture, actions[0], distThreshold) ? 1 : 0;
					action.setResult(i, gestureResult);
				}
			}
			else if (gesture->transitionType() == SimpleGesture::FixedMovement) {
				BodyPoint::BodyPart bp = gesture->bodyPoint();
				std::vector<ActionCommand*> actions = filterCommands(1, bp);
				if (!actions.empty()) {
					int gestureResult = meetConditions(gesture, actions[0], distThreshold) ? 1 : 2;
					action.setResult(i, gestureResult);
				}
			}
		}
	}

}