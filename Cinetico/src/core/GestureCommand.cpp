
#include "GestureCommandsManager.h"
#include "entity/Body.h"
#include "entity/SimpleGesture.h"

namespace cinetico_core {

	GestureCommand::GestureCommand(const BodyPointState &bodyPointState)
		: m_bp(bodyPointState.bodyPoint()) {
		m_initTime = bodyPointState.initTime();
		m_initPosition = bodyPointState.initialPosition();
		m_lastUpdateTime = bodyPointState.lastUpdateTime();
		m_endTime = 0;
		m_finished = false;
		m_active = true;
	}

	void GestureCommand::update(const BodyPointState &bodyPointState) {
		m_lastUpdateTime = bodyPointState.lastUpdateTime();
	}

	void GestureCommand::finish(uilib::u64 time) {
		m_finished = true;
		m_endTime = time;
	}

	void GestureCommand::setActive(bool active) {
		m_active = active;
	}



	MovementGestureCommand::MovementGestureCommand(PositionGestureCommand* transitionGesture, const BodyPointState &bodyPointState)
		: GestureCommand(bodyPointState) {
		m_currentDirection = m_bp.position() - m_initPosition;
		m_currentDirection.normalize();
		m_currentPosition = bodyPointState.lastPosition();
		if (transitionGesture)
			m_points.push_back(transitionGesture->initPosition());
		else
			m_points.push_back(m_currentDirection); //should never get here, but it does
	}
	
	void MovementGestureCommand::update(const BodyPointState &bodyPointState) {
		/*
		GestureCommand::update(bodyPointState);
		Vector3 oldDirection = m_currentDirection;
		Vector3 newDirection = bodyPointState.lastPosition() - m_currentPosition;
		newDirection.normalize();
		m_currentPosition = bodyPointState.lastPosition();
		//float dot = dotProduct(oldDirection, newDirection);
		//if (dot < 0)
			addPoint(m_currentPosition);
		//m_currentDirection = newDirection;
		*/
	}

	void MovementGestureCommand::addPoint(Vector3 point) {
		m_points.push_back(point);
	}



	PositionGestureCommand::PositionGestureCommand(MovementGestureCommand* transitionGesture, const BodyPointState &bodyPointState)
		: GestureCommand(bodyPointState) {
		m_transitionGesture = transitionGesture;
		m_transitionGesture->finish(bodyPointState.lastUpdateTime());
		m_holdTime = bodyPointState.holdTime();
		if (transitionGesture)
			transitionGesture->addPoint(m_initPosition);
	}
	
	void PositionGestureCommand::update(const BodyPointState &bodyPointState) {
		GestureCommand::update(bodyPointState);
		m_holdTime = bodyPointState.holdTime();
	}

	void PositionGestureCommand::finish(uilib::u64 time) {
		if (m_transitionGesture)
			m_transitionGesture->finish(time);
		this->GestureCommand::finish(time);
	}

}