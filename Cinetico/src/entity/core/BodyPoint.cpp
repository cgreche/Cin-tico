
#include "BodyPoint.h"

namespace cinetico_core {

	BodyPoint::BodyPoint(Body& ownerBody, BodyPart bodyPart, const char *tag)
		: m_ownerBody(ownerBody) {
		m_trackable = true;
		m_bodyPart = bodyPart;
		m_tag = tag;
		m_identified = false;
		m_occluded = false;
	}

}