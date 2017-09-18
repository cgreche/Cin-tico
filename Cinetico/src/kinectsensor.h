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

		// Kinect data
		bool m_identifiedBodies[BODY_COUNT];
		int m_identifiedBodyCount;
		Joint m_joints[BODY_COUNT][JointType_Count];
		JointOrientation m_orientations[BODY_COUNT][JointType_Count];

	public:
		KinectSensor();

		bool initialize();
		void update();
		void finalize();
		long checkCapabilities(long capabilities);
		u8 *getDepthImageData();

		int identifiedBodyCount() const { return m_identifiedBodyCount; }
		bool bodyIsidentified(int body) const { return m_identifiedBodies[body]; }
		Joint joint(int body, JointType joint) const { return m_joints[body][joint]; }
		JointOrientation jointOrientation(int body, JointType joint) const { return m_orientations[body][joint]; }
	};

}

#endif