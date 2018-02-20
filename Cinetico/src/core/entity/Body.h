#ifndef __CINETICO_CORE_BODY_H__
#define __CINETICO_CORE_BODY_H__

#include "BodyPoint.h"
#include "uilib/lib/time.h"

namespace cinetico_core {

	class Sensor;

	class Body {
		friend class BodyTracker;

	public:
		static const int BodyPointCount = BodyPoint::BodyPartCount;
		Sensor &m_sensor;
		int m_identifiedBodyPointCount;
		Quaternion m_orientation;

	private:
		BodyPoint *m_bodyPoint[BodyPointCount];
		uilib::u64 m_lastUpdateTime;

	public:
		Body(Sensor &sensor);

		void setBodyPointTrackable(BodyPoint::BodyPart bodyPart, bool trackable);
		void setAllBodyPointsTrackable(bool trackable);
		int identifiedBodyPointCount() const { return m_identifiedBodyPointCount; }
		BodyPoint *bodyPoint(BodyPoint::BodyPart bodyPart) const { return m_bodyPoint[bodyPart]; }
	};

}

#endif