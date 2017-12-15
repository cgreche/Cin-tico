
#include "GestureCommandsManager.h"
#include "entity/Body.h"
#include "entity/Action.h"
#include "entity/SimpleGesture.h"

namespace cinetico_core {

	GestureCommandsManager::GestureCommandsManager(float distThreshold, float minHoldtime) {
		m_distThreshold = distThreshold;
		m_minHoldtime = minHoldtime;
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
			m_trackedBps.push_back(BodyPointState(*body->bodyPoint((BodyPoint::BodyPart)i), 0, m_distThreshold, m_minHoldtime));
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
				//continue;
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

	bool GestureCommandsManager::meetConditions(SimpleGesture *gesture, GestureCommand *command) {
		float distThreshold = m_distThreshold;

		Vector3 actionPoint;
		Vector3 actionOrientation;
		//if (command->movementGestureCommand() && gesture->transitionType()==SimpleGesture::FixedMovement)
		//	return false; //we are going to check only positionGestures (movementgesture can be "parent")

		Vector3 targetPoint;
		Vector3 refPos;
		Vector3 frontDir;
		Vector3 rightDir;
		Vector3 upDir;

		int refPoint = gesture->refPoint();
		if (refPoint == -2) //Sensor
			refPos = Vector3(0, 0, 0);
		else if (refPoint == -1) //LastPos
			refPos = m_trackedBps[gesture->bodyPoint()].lastSteadyPosition(); //does nothing (todo)
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
			PositionGestureCommand *posCommand = command->positionGestureCommand();
			MovementGestureCommand *movCommand = NULL;
			if (posCommand) {
				movCommand = (MovementGestureCommand*)posCommand->transitionGestureCommand();
				if (movCommand == NULL)
					return false;
			}
			else
				movCommand = command->movementGestureCommand();

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
		bool result;
		if (op == SimpleGesture::InFront) {
			result = actionPoint.z() < targetPoint.z();
		}
		else if (op == SimpleGesture::Behind) {
			result = actionPoint.z() > targetPoint.z();
		}
		else if (op == SimpleGesture::ToRight) {
			result = actionPoint.x() > targetPoint.x();
		}
		else if (op == SimpleGesture::ToLeft) {
			result = actionPoint.x() > targetPoint.x();
		}
		else if (op == SimpleGesture::Above) {
			result = actionPoint.y() > targetPoint.y();
		}
		else if (op == SimpleGesture::Below) {
			result = actionPoint.y() < targetPoint.y();
		}
		else if (op == SimpleGesture::FixedPosition) {
			result = actionPoint.euclideanDistanceTo(targetPoint) <= distThreshold;
		}
		else if (op == SimpleGesture::AtSameBreadth) {
			result = fabsf(actionPoint.x() - targetPoint.x()) <= distThreshold;
		}
		else if (op == SimpleGesture::AtSameHeight) {
			result = fabsf(actionPoint.y() - targetPoint.y()) <= distThreshold;
		}
		else if (op == SimpleGesture::AtSameDepth) {
			result = fabsf(actionPoint.z() - targetPoint.z()) <= distThreshold;
		}
		else if (op == SimpleGesture::FixedOrientation) {
			//todo
			Vector3 diff = targetPoint - actionPoint;
			diff.normalize();
			result = dotProduct(actionOrientation, diff) >= 0.8f;

		}
		else if (op == SimpleGesture::OrientationLookingTo) {
			//todo
			Vector3 diff = targetPoint - actionPoint;
			diff.normalize();
			result = dotProduct(actionOrientation, diff) >= 0.8f;
		}
		else {
			result = false;
		}

		//checar constraint de velocidade
		if (result && command->positionGestureCommand() && gesture->transitionType() == SimpleGesture::FixedMovement) {
			MovementGestureCommand *movGesture = command->positionGestureCommand()->transitionGestureCommand();
			if (movGesture) {
				uilib::u64 initTime = movGesture->initTime();
				uilib::u64 lastTime = movGesture->lastUpdateTime();
				Vector3 initPos = movGesture->initPosition();
				Vector3 endPos = command->positionGestureCommand()->initPosition();
				Vector3 diff = (initPos - endPos);
				float dt = (double)(lastTime - initTime)/uilib::OSTime::ticksPerSecond();
				float speed = fabsf(diff.length()) / dt;

				float minSpeed = ((MovementGesture*)gesture)->minSpeed();
				float maxSpeed = ((MovementGesture*)gesture)->maxSpeed();
				result = speed >= minSpeed && (speed <= maxSpeed || maxSpeed == 0.f);
			}
		}

		return result;
	}

	void GestureCommandsManager::checkConditions(Action &action) {
		int gestureCount = action.gestureCount();

		for (int i = 0; i < gestureCount; ++i) {
			SimpleGesture *gesture = action.gesture(i);
			BodyPoint::BodyPart bp = gesture->bodyPoint();
			std::vector<GestureCommand*> steadyCommands = filterCommands(BodyPointState::STEADY, bp);
			std::vector<GestureCommand*> movCommands = filterCommands(BodyPointState::MOVING, bp);
			if (!steadyCommands.empty()) {
				
				if (gesture->transitionType() == SimpleGesture::FixedMovement) {
					if (!steadyCommands[0]->active())
						continue;
				}
				
				int gestureResult = meetConditions(gesture, steadyCommands[0]) ? 1 : 0;
				//if condition wasn't met and it is Free transition, ignore;
				//otherwise, set result
				if (gestureResult == 0 && gesture->transitionType() == SimpleGesture::FixedMovement && steadyCommands[0]->active())
					gestureResult = 2;

				action.setGestureResult(i, gestureResult);
				if (gestureResult != 0)
					steadyCommands[0]->setActive(false);

			}
			else if (!movCommands.empty()) {
				//int gestureResult = meetConditions(gesture, movCommands[0]) ? 1 : 0;
				//if condition wasn't met and it is Free transition, ignore;
				//otherwise, set result
				//action.setGestureResult(i, gestureResult);
			}
		}
	}

}