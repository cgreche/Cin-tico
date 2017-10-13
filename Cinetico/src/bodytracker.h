#ifndef __CINETICO_CORE_BODYTRACKER_H__
#define __CINETICO_CORE_BODYTRACKER_H__

#include "sensor.h"
#include "entity/core/Body.h"

namespace cinetico_core {

	class BodyTracker {
		Sensor &m_sensor;
		Body *m_body;

		unsigned long m_trackableBodyPoints;

		int m_identifiedBodyCount;

		public:
			BodyTracker(Sensor& sensor);
			~BodyTracker();
			bool track();
			void setTrackableBodyPoints(unsigned long trackableBodyPoints) { m_trackableBodyPoints = trackableBodyPoints; }
			Body *body() const { return m_body; }
	};

}

#endif