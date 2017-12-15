#ifndef __CINETICO_CORE_BODY_POINT_STATE_H__
#define __CINETICO_CORE_BODY_POINT_STATE_H__

#include "entity/BodyPoint.h"
#include "uilib/lib/time.h"

namespace cinetico_core {

	class BodyPointState {
	public:
		enum GestureState {
			NOT_IDENTIFIED,
			STEADY,
			MOVING
		};

	private:
		float m_distThreshold;
		uilib::u64 m_minHoldtime;

		BodyPoint &m_bp;
		uilib::u64 m_lastUpdateTime;
		uilib::u64 m_initTime;
		uilib::u64 m_holdTime;
		Vector3 m_initalPosition;
		Vector3 m_lastPosition;
		Vector3 m_lastSteadyPos;

		GestureState m_gestureState;

	public:
		BodyPointState(BodyPoint &bp, uilib::u64 initTime, float distThreshold = 0.045f, float minHoldtime = 0.25f);
		GestureState update(uilib::u64 curTime);

		BodyPoint& bodyPoint() const { return m_bp; }
		uilib::u64 lastUpdateTime() const { return m_lastUpdateTime; }
		uilib::u64 initTime() const { return m_holdTime; }
		uilib::u64 holdTime() const { return m_holdTime; }
		const Vector3 &initialPosition() const { return m_initalPosition; }
		const Vector3 &lastPosition() const { return m_lastPosition; }
		const Vector3 &lastSteadyPosition() const { return m_lastSteadyPos; }
	};

}

#endif