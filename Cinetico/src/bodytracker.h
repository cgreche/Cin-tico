#ifndef __CINETICO_CORE_BODYTRACKER_H__
#define __CINETICO_CORE_BODYTRACKER_H__

#include "sensor.h"
#include "entity/core/Body.h"

namespace cinetico_core {

	class BodyTracker {
		Sensor &m_sensor;
		Body *m_body;

		int m_identifiedBodyCount;

		public:
			BodyTracker(Sensor& sensor);
			bool track();
			Body *body() const { return m_body; }
	};

}



#endif