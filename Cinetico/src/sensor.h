#ifndef __CINETICO_CORE_SENSOR_H__
#define __CINETICO_CORE_SENSOR_H__

typedef unsigned char u8;

namespace cinetico_core {

	class Sensor {
		public:
			virtual bool initialize() = 0;
			virtual void update() = 0;
			virtual void finalize() = 0;
			virtual long checkCapabilities(long capabilities) = 0;
			virtual u8 *getDepthImageData() = 0;
	};

}

#endif