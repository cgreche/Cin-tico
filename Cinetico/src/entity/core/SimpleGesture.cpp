
#include "SimpleGesture.h"
#include "Exercise.h"
#include <uilib/lib/time.h>

namespace cinetico_core {

	SimpleGesture::SimpleGesture(TransitionType type, BodyPoint::BodyPart bodyPoint) {
		m_type = type;
		m_bodyPoint = bodyPoint;
		m_refPoint = -2;
		m_operation = FixedPosition;
	}

}