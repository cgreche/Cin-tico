
#include "BodyPointState.h"

namespace cinetico_core {

	BodyPointState::BodyPointState(BodyPoint &bp, uilib::u64 initTime, float distThreshold, float minHoldtime)
		: m_bp(bp) {
		m_distThreshold = distThreshold;
		m_minHoldtime = (uilib::u64)(uilib::OSTime::ticksPerSecond()*minHoldtime);

		m_lastUpdateTime = initTime;
		m_initalPosition = m_lastPosition = bp.position();
		m_holdTime = 0;
		m_initTime = initTime;
		m_gestureState = NOT_IDENTIFIED;
	}

	BodyPointState::GestureState BodyPointState::update(uilib::u64 curTime) {
		m_lastUpdateTime = curTime;

		Vector3 curPos = m_bp.position();
		GestureState gestureState = m_gestureState;

		if (curPos.euclideanDistanceTo(m_initalPosition) < m_distThreshold) {
			m_holdTime = curTime - m_initTime;
			if (gestureState != STEADY && m_holdTime >= m_minHoldtime) {
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
}