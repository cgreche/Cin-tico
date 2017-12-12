#ifndef __CINETICO_CORE_ACTION_COMMANDS_MANAGER_H__
#define __CINETICO_CORE_ACTION_COMMANDS_MANAGER_H__

#include <vector>
#include "entity/BodyPoint.h"
#include "entity/Action.h"
#include "lib/vector3.h"
#include "uilib/lib/time.h"

#define GAP 0.06f

namespace cinetico_core {

	class Action;
	class PositionGestureCommand;
	class MovementGestureCommand;

	class BodyPointState {
	public:
		enum GestureState {
			NOT_IDENTIFIED,
			STEADY,
			MOVING
		};

	private:
		BodyPoint &m_bp;
		uilib::u64 m_lastUpdateTime;
		uilib::u64 m_initTime;
		uilib::u64 m_holdTime;
		Vector3 m_initalPosition;
		Vector3 m_lastPosition;

		GestureState m_gestureState;

	public:
		BodyPointState(BodyPoint &bp, uilib::u64 initTime)
			: m_bp(bp) {
			m_lastUpdateTime = initTime;
			m_initalPosition = m_lastPosition = bp.position();
			m_holdTime = 0;
			m_initTime = initTime;
			m_gestureState = NOT_IDENTIFIED;
		}

		GestureState update(uilib::u64 curTime) {
			uilib::u64 minHoldTime = (uilib::u64)(uilib::OSTime::ticksPerSecond() / 4);

			m_lastUpdateTime = curTime;

			Vector3 curPos = m_bp.position();
			GestureState gestureState = m_gestureState;

			if (curPos.euclideanDistanceTo(m_initalPosition) < GAP) {
				m_holdTime = curTime - m_initTime;
				if (gestureState != STEADY && m_holdTime >= minHoldTime) {
					m_initTime = curTime;
					gestureState = STEADY;
				}
			}
			else {
				m_holdTime = 0;
				m_initalPosition = curPos;
				m_initTime = curTime;
				if (m_gestureState == STEADY)
					gestureState = MOVING;
			}

			m_lastPosition = curPos;
			return m_gestureState = gestureState;
		}

		BodyPoint& bodyPoint() const { return m_bp; }
		uilib::u64 lastUpdateTime() const { return m_lastUpdateTime; }
		uilib::u64 initTime() const { return m_holdTime; }
		uilib::u64 holdTime() const { return m_holdTime; }
		const Vector3 &initialPosition() const { return m_initalPosition; }
		const Vector3 &lastPosition() const { return m_lastPosition; }
	};

	class GestureCommand {
	protected:
		BodyPoint &m_bp;
		uilib::u64 m_initTime;
		uilib::u64 m_endTime;
		uilib::u64 m_lastUpdateTime;
		Vector3 m_initPosition;
		bool m_finished;

	public:
		GestureCommand(const BodyPointState &bodyPointState)
			: m_bp(bodyPointState.bodyPoint()) {
				m_initTime = bodyPointState.initTime();
				m_initPosition = bodyPointState.initialPosition();
				m_lastUpdateTime = bodyPointState.lastUpdateTime();
				m_endTime = 0;
				m_finished = false;
			}

			virtual PositionGestureCommand *positionGestureCommand() { return NULL; }
			virtual MovementGestureCommand *movementGestureCommand() { return NULL; }

			virtual void update(const BodyPointState &bodyPointState) {
				m_lastUpdateTime = bodyPointState.lastUpdateTime();
			}

			virtual void finish(uilib::u64 time) {
				m_finished = true;
				m_endTime = time;
			}

			BodyPoint &bodyPoint() { return m_bp; }
			uilib::u64 initTime() const { return m_initTime; }
			uilib::u64 endTime() const { return m_endTime; }
			uilib::u64 lastUpdateTime() const { return m_lastUpdateTime; }
			Vector3 initPosition() const { return m_initPosition; }
			bool finished() const { return m_finished; }
	};

	class PositionGestureCommand : public GestureCommand {
		MovementGestureCommand *m_transitionGesture;
		uilib::u64 m_holdTime;

	public:
		PositionGestureCommand(MovementGestureCommand* transitionGesture, const BodyPointState &bodyPointState)
			: GestureCommand(bodyPointState) {
			m_transitionGesture = transitionGesture;
			m_holdTime = bodyPointState.holdTime();
		}

		virtual PositionGestureCommand *positionGestureCommand() { return this; }

		virtual void update(const BodyPointState &bodyPointState) {
			GestureCommand::update(bodyPointState);
			m_holdTime = bodyPointState.holdTime();
		}

		virtual void finish(uilib::u64 time) {
			m_transitionGesture->finish(time);
			this->GestureCommand::finish(time);
		}

		uilib::u64 holdTime() const { return m_holdTime; }
	};

	class MovementGestureCommand : public GestureCommand {
		cinetico_core::Vector3 m_currentDirection;
		cinetico_core::Vector3 m_currentPosition;
		std::vector<Vector3> m_points;

	public:
		MovementGestureCommand(const BodyPointState &bodyPointState)
			: GestureCommand(bodyPointState) {
			m_currentDirection = m_bp.position() - m_initPosition;
			m_currentPosition = bodyPointState.lastPosition();
			m_currentDirection.normalize();
		}

		virtual MovementGestureCommand *movementGestureCommand() { return this; }

		virtual void update(const BodyPointState &bodyPointState) {
			GestureCommand::update(bodyPointState);
			Vector3 oldDirection = m_currentDirection;
			Vector3 newDirection = m_bp.position() - m_initPosition;
			m_currentPosition = bodyPointState.lastPosition();
			//to-do: calculate direction change (dotProduct)
			m_currentDirection = newDirection;
		}

		const Vector3 &currentPosition() const { return m_currentPosition; }

	};

	class GestureCommandsManager {
		Body *m_body;
		std::vector<GestureCommand*> m_gestureCommands;
		std::vector<BodyPointState> m_trackedBps;

		uilib::u64 m_curTime;

		bool meetConditions(SimpleGesture *gesture, GestureCommand *command, float distThreshold);
	public:
		GestureCommandsManager();
		~GestureCommandsManager();

		void reset();
		void step(uilib::u64 curTime);

		unsigned int commandCount() const { return m_gestureCommands.size(); }
		GestureCommand *actionCommand(int i) const { return m_gestureCommands[i]; }
		void setBody(Body *body);

		std::vector<GestureCommand *> filterCommands(int transitionType, BodyPoint::BodyPart bp);

		void checkConditions(Action &action, float distThreshold);
	};

}

#endif