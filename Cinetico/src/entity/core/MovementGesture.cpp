
#include "MovementGesture.h"

namespace cinetico_core {

	MovementGesture::MovementGesture(BodyPoint::BodyPart bodyPoint, MovementType movementType)
		: SimpleGesture(FixedMovement, bodyPoint) {
		m_movementType = movementType;
		m_minSpeed = m_maxSpeed = 0.f;
	}

}
