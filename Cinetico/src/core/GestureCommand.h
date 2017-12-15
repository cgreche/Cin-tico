#ifndef __CINETICO_CORE_GESTURE_COMMAND_H__
#define __CINETICO_CORE_GESTURE_COMMAND_H__

#include <vector>
#include "entity/BodyPoint.h"
#include "entity/Action.h"
#include "lib/vector3.h"
#include "uilib/lib/time.h"

#include "BodyPointState.h"

#define GAP 0.06f

namespace cinetico_core {

	class Action;
	class PositionGestureCommand;
	class MovementGestureCommand;

	class GestureCommand {
	protected:
		BodyPoint &m_bp;
		uilib::u64 m_initTime;
		uilib::u64 m_endTime;
		uilib::u64 m_lastUpdateTime;
		Vector3 m_initPosition;
		bool m_active;
		bool m_finished;

	public:
		GestureCommand(const BodyPointState &bodyPointState);
		
		virtual PositionGestureCommand *positionGestureCommand() { return NULL; }
		virtual MovementGestureCommand *movementGestureCommand() { return NULL; }
		
		virtual void update(const BodyPointState &bodyPointState);

		virtual void finish(uilib::u64 time);

		void setActive(bool active);

		BodyPoint &bodyPoint() { return m_bp; }
		uilib::u64 initTime() const { return m_initTime; }
		uilib::u64 endTime() const { return m_endTime; }
		uilib::u64 lastUpdateTime() const { return m_lastUpdateTime; }
		Vector3 initPosition() const { return m_initPosition; }
		bool finished() const { return m_finished; }
		bool active() const { return m_active; }
	};

	class MovementGestureCommand : public GestureCommand {
		cinetico_core::Vector3 m_currentDirection;
		cinetico_core::Vector3 m_currentPosition;
		std::vector<Vector3> m_points;

	public:
		MovementGestureCommand(PositionGestureCommand* transitionGesture, const BodyPointState &bodyPointState);

		virtual MovementGestureCommand *movementGestureCommand() { return this; }
		virtual void update(const BodyPointState &bodyPointState);
		void addPoint(Vector3 point);

		const Vector3 &currentPosition() const { return m_currentPosition; }
		const Vector3 &endPosition() const { return m_points[m_points.size() - 1]; }

		int pointCount() const { return m_points.size(); }
		const Vector3 &point(int index) const { return m_points[index]; }
	};

	class PositionGestureCommand : public GestureCommand {
		MovementGestureCommand *m_transitionGesture;
		uilib::u64 m_holdTime;

	public:
		PositionGestureCommand(MovementGestureCommand* transitionGesture, const BodyPointState &bodyPointState);

		virtual PositionGestureCommand *positionGestureCommand() { return this; }
		virtual void update(const BodyPointState &bodyPointState);
		virtual void finish(uilib::u64 time);

		MovementGestureCommand *transitionGestureCommand() const { return m_transitionGesture; }
		uilib::u64 holdTime() const { return m_holdTime; }
	};

}

#endif