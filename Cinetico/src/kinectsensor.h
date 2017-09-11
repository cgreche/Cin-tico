#ifndef __CINETICO_CORE_KINECTICSENSOR_H__
#define __CINETICO_CORE_KINECTICSENSOR_H__

#include "sensor.h"
#include <Kinect.h>

namespace cinetico_core {

	class KinectSensor : public Sensor
	{
		static const int cDepthWidth = 512;
		static const int cDepthHeight = 424;

		// Current Kinect
		IKinectSensor* m_pKinectSensor;
		ICoordinateMapper* m_pCoordinateMapper;

		// Body reader
		IBodyFrameReader* m_pBodyFrameReader;

	public:
		KinectSensor();

		bool initialize();
		void update();
		void finalize();
		long checkCapabilities(long capabilities);
		u8 *getDepthImageData();
	};

}

#endif