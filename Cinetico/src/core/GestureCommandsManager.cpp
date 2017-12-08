
#include "GestureCommandsManager.h"
#include "entity/Body.h"
#include "entity/SimpleGesture.h"

namespace cinetico_core {

	GestureCommandsManager::GestureCommandsManager() {
	}

	GestureCommandsManager::~GestureCommandsManager() {
		for (unsigned int i = 0; i < m_gestureCommands.size(); ++i) {
			delete m_gestureCommands[i];
		}
	}

	void GestureCommandsManager::reset() {
		if (!m_gestureCommands.empty())
			m_gestureCommands.clear();
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
		m_curTime = curTime;
		if (!m_body)
			return;

		if (!m_gestureCommands.empty())
			m_gestureCommands.clear();

		for (unsigned int i = 0; i < BodyPoint::BodyPartCount; ++i) {
			BodyPointState::GestureState gestureState = m_trackedBps[i].update(curTime);
			std::vector<GestureCommand*> gestureCommands;
			if (gestureState != BodyPointState::NOT_IDENTIFIED) {
				gestureCommands = filterCommands(gestureState, (BodyPoint::BodyPart)i);
				if (gestureCommands.size() > 0) {
					gestureCommands[0]->update(m_trackedBps[i]);
				}
			}
			else {
				if (gestureState == BodyPointState::STAYED) {
					m_gestureCommands.push_back(new PositionGestureCommand(m_trackedBps[i]));
				}
				else if (gestureState == BodyPointState::MOVING) {
					m_gestureCommands.push_back(new MovementGestureCommand(m_trackedBps[i]));
				}
			}
		}
		/*
		ActionCommand *actionCommand = m_trackedBps[0].update(curTime);
		if (actionCommand)
			m_commands.push_back(actionCommand);
		*/
	}



	std::vector<GestureCommand*> GestureCommandsManager::filterCommands(int transitionType, BodyPoint::BodyPart bp) {
		std::vector<GestureCommand*> ret;
		for (GestureCommand *command : m_gestureCommands) {
			if (transitionType == 0 && command->positionGestureCommand()) {
				if ((command->initTime() == m_curTime || command->lastUpdateTime() == m_curTime)
					&& command->bodyPoint().bodyPart() == bp)
					ret.push_back(command);
			}
			else if (transitionType == 1 && command->movementGestureCommand()) {
				if (command->endTime() == m_curTime && command->bodyPoint().bodyPart() == bp)
					ret.push_back(command);
			}
		}
		return ret;
	}

	bool GestureCommandsManager::meetConditions(SimpleGesture *gesture, GestureCommand *command, float distThreshold) {
		//todo: handle refPoint Any or LastPosition

		Vector3 actionPoint;
		if (command->movementGestureCommand())
			actionPoint = command->movementGestureCommand()->endPosition();
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
				std::vector<GestureCommand*> actions = filterCommands(0, bp);
				if (!actions.empty()) {
					int gestureResult = meetConditions(gesture, actions[0], distThreshold) ? 1 : 0;
					action.setResult(i, gestureResult);
				}
			}
			else if (gesture->transitionType() == SimpleGesture::FixedMovement) {
				BodyPoint::BodyPart bp = gesture->bodyPoint();
				std::vector<GestureCommand*> actions = filterCommands(1, bp);
				if (!actions.empty()) {
					int gestureResult = meetConditions(gesture, actions[0], distThreshold) ? 1 : 2;
					action.setResult(i, gestureResult);
				}
			}
		}
	}

}