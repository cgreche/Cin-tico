#ifndef __CINETICO_CORE_ACTION_COMMANDS_MANAGER_H__
#define __CINETICO_CORE_ACTION_COMMANDS_MANAGER_H__

#include <vector>
#include "BodyPoint.h"
#include "Action.h"
#include "vector3.h"
#include "uilib/lib/time.h"

namespace cinetico_core {

	class Action;
	class PositionActionCommand;
	class MovementActionCommand;

	class ActionCommand {
	protected:
		BodyPoint &m_bp;
		uilib::u64 m_initTime;
		uilib::u64 m_endTime;
		uilib::u64 m_lastUpdateTime;
		Vector3 m_initPosition;
		bool m_finished;

	public:
		ActionCommand(BodyPoint &bp, uilib::u64 initTime, const Vector3 initPosition)
			: m_bp(bp) {
			m_initTime = initTime;
			m_initPosition = initPosition;
			m_endTime = 0;
			m_finished = false;
		}

		virtual PositionActionCommand *positionActionCommand() { return NULL; }
		virtual MovementActionCommand *movementActionCommand() { return NULL; }

		virtual bool update(uilib::u64 curTime) = 0;

		void finish(uilib::u64 time) {
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

	class PositionActionCommand : public ActionCommand {

		uilib::u64 m_holdTime;

	public:
		PositionActionCommand(BodyPoint &bp, uilib::u64 initTime, const cinetico_core::Vector3 &initPosition, uilib::u64 holdTime)
			: ActionCommand(bp, initTime, initPosition) {
			m_holdTime = holdTime;
		}

		virtual PositionActionCommand *positionActionCommand() { return this; }

		virtual bool update(uilib::u64 curTime) {
			float gap = 0.04f;
			m_lastUpdateTime = curTime;
			cinetico_core::Vector3 curPos = m_bp.position();
			if (curPos.euclideanDistanceTo(m_initPosition) < gap) {
				m_holdTime = curTime - m_initTime;
				return false;
			}
			else {
				finish(curTime);
				return true;
			}
		}

		cinetico_core::Vector3 initPosition() const { return m_initPosition; }
		uilib::u64 holdTime() const { return m_holdTime; }
	};

	class MovementActionCommand : public ActionCommand {
		cinetico_core::Vector3 m_currentOrientation;
		cinetico_core::Vector3 m_endPosition;
		std::vector<Vector3> m_points;

	public:
		MovementActionCommand(BodyPoint &bp, uilib::u64 initTime, const Vector3 &initPosition)
			: ActionCommand(bp, initTime, initPosition) {
			m_currentOrientation = bp.position() - initPosition;
			m_currentOrientation.normalize();
		}

		virtual MovementActionCommand *movementActionCommand() { return this; }

		virtual bool update(uilib::u64 curTime) {
			float gap = 0.04f;
			static float holdTime = 0.5f;
			m_lastUpdateTime = curTime;
			static uilib::u64 minHoldTicks = (uilib::u64)(uilib::OSTime::ticksPerSecond() / 4);
			float distOnLine = m_bp.position().euclideanDistanceToLine(m_initPosition, m_currentOrientation);
			float dist = m_bp.position().euclideanDistanceTo(m_initPosition);
			//id dist < lastDist, then user wen't in the opposition direction
			if (distOnLine > gap) {
				finish(curTime);
				return true;
			}
			return false;
		}

		Vector3 endPosition() const { return m_endPosition; }

	};

	class BodyPointState {
		BodyPoint &m_bp;
		uilib::u64 m_initTime;
		uilib::u64 m_holdTime;
		Vector3 m_lastPosition;

		ActionCommand *m_currentAction;


	public:
		BodyPointState(BodyPoint &bp, uilib::u64 initTime)
			: m_bp(bp) {
			m_lastPosition = bp.position();
			m_holdTime = 0;
			m_initTime = initTime;
			m_currentAction = NULL;
		}

		ActionCommand* update(uilib::u64 curTime) {
			static float minHoldTime = 0.5f;
			float gap = 0.04f;
			uilib::u64 minHoldTicks = (uilib::u64)(uilib::OSTime::ticksPerSecond()/4);

			Vector3 curPos = m_bp.position();

			if (curPos.euclideanDistanceTo(m_lastPosition) < gap)
 				m_holdTime = curTime - m_initTime;
			else {
				m_initTime = curTime;
				m_holdTime = 0;
				m_lastPosition = curPos;
			}

			bool result = false;
			if (m_currentAction) {
				result = m_currentAction->update(curTime);
				if (m_currentAction->movementActionCommand() && !result) {
					MovementActionCommand *curAction = m_currentAction->movementActionCommand();
					if (m_holdTime >= minHoldTicks) {
						curAction->finish(curTime);
						result = true;
					}
				}
			}

			if (result) {
				if (m_currentAction && m_currentAction->positionActionCommand()) {
					return m_currentAction = new MovementActionCommand(m_bp, curTime, m_lastPosition);
				}

				m_currentAction = NULL;
			}

			if (m_holdTime >= minHoldTicks)
				return m_currentAction = new PositionActionCommand(m_bp, curTime, m_lastPosition, m_holdTime);
			return NULL;
		}

		const BodyPoint& bodyPoint() const { return m_bp; }
		uilib::u64 holdTime() { return m_holdTime; }
	};


	class GestureCommandsManager {
		Body *m_body;
		std::vector<ActionCommand*> m_commands;
		std::vector<BodyPointState> m_trackedBps;

		uilib::s64 m_curTime;

		bool meetConditions(SimpleGesture *gesture, ActionCommand *command, float distThreshold);
	public:
		GestureCommandsManager();
		~GestureCommandsManager();

		void reset();
		void step(uilib::s64 curTime);

		unsigned int commandCount() const { return m_commands.size(); }
		ActionCommand *actionCommand(int i) const { return m_commands[i]; }
		void setBody(Body *body);

		std::vector<ActionCommand *> filterCommands(int transitionType, BodyPoint::BodyPart bp);

		void checkConditions(Action &action, float distThreshold);
	};

}

#endif