#ifndef __CINETICO_CORE_SENSOR_H__
#define __CINETICO_CORE_SENSOR_H__

typedef unsigned char u8;

namespace cinetico_core {

	class Sensor {
		public:
			enum Caps {
				DEPTH_IMAGE = 1,
				SKELETON_TRACKING = 2,
			};

			virtual ~Sensor() { }

			virtual int test() = 0;
			virtual bool initialize() = 0;
			virtual void update() = 0;
			virtual void finalize() = 0;
			virtual unsigned long getCapabilities() = 0;
			virtual unsigned char *getDepthImageData() = 0;
	};

}

#endif