
#include "GestureCommandsManager.h"
#include "entity/Body.h"
#include "entity/Action.h"
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

		for (int i = 0; i < m_gestureCommands.size();) {
			if (m_gestureCommands[i]->finished())
				m_gestureCommands.erase(m_gestureCommands.begin() + i);
			else
				++i;
		}

		for (unsigned int i = 0; i < BodyPoint::BodyPartCount; ++i) {
			if (i == BodyPoint::RightPalm)
				int a = 1;
			else {
			//	continue;
			}

			BodyPointState::GestureState gestureState = m_trackedBps[i].update(curTime);
			std::vector<GestureCommand*> gestureCommands;

			if (gestureState != BodyPointState::NOT_IDENTIFIED) {
				std::vector<GestureCommand*> posGesture = filterCommands(BodyPointState::STEADY, (BodyPoint::BodyPart)i);
				std::vector<GestureCommand*> movGesture = filterCommands(BodyPointState::MOVING, (BodyPoint::BodyPart)i);
				if (gestureState == BodyPointState::STEADY) {
					if (posGesture.size() > 0)
						posGesture[0]->update(m_trackedBps[i]);
					else {
						MovementGestureCommand* transitionGesture = NULL;
						if (movGesture.size() > 0)
							transitionGesture = (MovementGestureCommand*)movGesture[0];
						m_gestureCommands.push_back(new PositionGestureCommand(transitionGesture,m_trackedBps[i]));
					}
				}
				else if (gestureState == BodyPointState::MOVING) {
					if (posGesture.size() == 0 && movGesture.size() > 0)
						movGesture[0]->update(m_trackedBps[i]);
					else {
						PositionGestureCommand* transitionGesture = NULL;
						if (posGesture.size() > 0) {
							transitionGesture = (PositionGestureCommand*)posGesture[0];
							posGesture[0]->finish(curTime);
						}
						m_gestureCommands.push_back(new MovementGestureCommand(transitionGesture,m_trackedBps[i]));
					}
				}
			}
		}
	}



	std::vector<GestureCommand*> GestureCommandsManager::filterCommands(int type, BodyPoint::BodyPart bp) {
		std::vector<GestureCommand*> ret;
		for (GestureCommand *command : m_gestureCommands) {
			if (type == BodyPointState::STEADY && command->positionGestureCommand()) {
				if (command->bodyPoint().bodyPart() == bp)
					ret.push_back(command);
			}
			else if (type == BodyPointState::MOVING && command->movementGestureCommand()) {
				if (command->bodyPoint().bodyPart() == bp)
					ret.push_back(command);
			}
		}
		return ret;
	}

	bool GestureCommandsManager::meetConditions(SimpleGesture *gesture, GestureCommand *command, float distThreshold) {
		//todo: handle refPoint Any or LastPosition

		Vector3 actionPoint;
		Vector3 actionOrientation;
		if (command->movementGestureCommand())
			return false; //we are going to check only positionGestures (movementgesture can be "parent")

		Vector3 targetPoint;
		Vector3 refPos;
		Vector3 frontDir;
		Vector3 rightDir;
		Vector3 upDir;

		int refPoint = gesture->refPoint();
		if (refPoint == -2) //Sensor
			targetPoint = Vector3(0, 0, 0);
		else if (refPoint == -1) //LastPos
			; //does nothing (todo)
		else {
			BodyPoint *ref = m_body->bodyPoint((BodyPoint::BodyPart)gesture->refPoint());
			refPos = ref->position();
			Vector3 refOrientation = ref->orientation().rotatedVector(Vector3(0,1,0));
			frontDir = refOrientation;
			rightDir = crossProduct(frontDir, Vector3(0, 1, 0));
			//upDir = crossProduct(rightDir, frontDir);
			upDir = Vector3(0, 1, 0);
		}

		//Check for movement constraint
		if (gesture->transitionType() == SimpleGesture::FixedMovement) {
			MovementGesture *movGesture = (MovementGesture*)gesture;
			PositionGestureCommand *posCommand = (PositionGestureCommand*)command;
			MovementGestureCommand *movCommand = (MovementGestureCommand*)posCommand->transitionGestureCommand();
			if (movCommand == NULL)
				return false;
			if (movGesture->movementType() == MovementGesture::Linear) {
				if (movCommand->pointCount() > 2)
					return false;
				//passed movement constraint
			}
			else {
				//todo: check for smooth movement
			}

			actionPoint = movCommand->endPosition();

		}
		else {
			actionPoint = command->initPosition();
		}


		//Check for positioning constraint
		unsigned long operation = gesture->operation();
		int op = operation & 255;
		int posFlags = (operation >> 8) & 7;
		Vector3 finalPosition = gesture->value();
		if (posFlags == 0)
			return false;

		targetPoint = refPos;
		if (posFlags & 1) {
			targetPoint += rightDir*finalPosition.x();
		}

		if (posFlags & 2) {
			targetPoint += upDir*finalPosition.y();
		}

		if (posFlags & 4) {
			targetPoint += frontDir*finalPosition.z();
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
			Vector3 diff = targetPoint - actionPoint;
			diff.normalize();
			return dotProduct(actionOrientation, diff) >= 0.8f;

		}
		else if (op == SimpleGesture::OrientationLookingTo) {
			//todo
			Vector3 diff = targetPoint - actionPoint;
			diff.normalize();
			return dotProduct(actionOrientation, diff) >= 0.8f;
		}

		return false;
	}

	void GestureCommandsManager::checkConditions(uilib::u64 curTime, Action &action, float distThreshold) {
		int gestureCount = action.gestureCount();

		for (int i = 0; i < gestureCount; ++i) {
			SimpleGesture *gesture = action.gesture(i);
			BodyPoint::BodyPart bp = gesture->bodyPoint();
			std::vector<GestureCommand*> actions = filterCommands(BodyPointState::STEADY, bp);
			if (!actions.empty()) {
				if (gesture->transitionType() == SimpleGesture::FixedMovement && actions[0]->endTime() != curTime)
					continue;
				int gestureResult = meetConditions(gesture, actions[0], distThreshold) ? 1 : 0;
				//if condition wasn't met and it is Free transition, ignore;
				//otherwise, set result
				if (gestureResult == 0 && gesture->transitionType() == SimpleGesture::FixedMovement)
					gestureResult = 2;

				action.setGestureResult(i, gestureResult);
				if (gestureResult != 0)
					actions[0]->setActive(false);

			}
		}
	}

}