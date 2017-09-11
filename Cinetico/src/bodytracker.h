#ifndef __CINETICO_CORE_BODYTRACKER_H__
#define __CINETICO_CORE_BODYTRACKER_H__

#include "sensor.h"
#include "entity/core/Body.h"

namespace cinetico_core {

	class BodyTracker {
		Sensor &m_sensor;
		Body *m_body;

		public:
			BodyTracker(Sensor &sensor)
			: m_sensor(sensor) {

			}

			bool track();
			Body &body();
	};

}



#endif