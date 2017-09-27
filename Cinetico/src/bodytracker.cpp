
#include "bodytracker.h"

#ifdef _WIN32
#include "kinectsensor.h"
#endif

namespace cinetico_core {

	BodyTracker::BodyTracker(Sensor &sensor)
		: m_sensor(sensor) {
		m_identifiedBodyCount = 0;
	}

	bool BodyTracker::track() {
		int identifiedBodyPointCount = 0;

#ifdef _WIN32
#define KinectJoint2CineticoBodyPoint(bp,kj) \
do { \
	BodyPoint *p = m_body->m_bodyPoint[bp]; \
	if(p->trackable()) { \
		Joint joint = sensor.joint(identifiedBody, kj); \
		JointOrientation jointOrientation = sensor.jointOrientation(identifiedBody, kj); \
		Vector3 position = Vector3(-joint.Position.X, joint.Position.Y, joint.Position.Z); \
		Vector3 orientation = Vector3(jointOrientation.Orientation.x, jointOrientation.Orientation.y, jointOrientation.Orientation.z); \
		p->m_position = position; \
		p->m_orientation = orientation; \
		p->m_identified = joint.TrackingState != TrackingState_NotTracked; \
		p->m_occluded = joint.TrackingState != TrackingState_Tracked; \
		if(p->m_identified) identifiedBodyPointCount++; \
	} \
} while(0)

		KinectSensor &sensor = (KinectSensor&)m_sensor;
		sensor.update();

		m_identifiedBodyCount = sensor.identifiedBodyCount();
		if(m_identifiedBodyCount != 1) {
			if (m_body)
				m_body->m_identifiedBodyPointCount = 0;
			return false;
		}

		if (!m_body)
			m_body = new Body(sensor);

		int identifiedBody;
		for(int i = 0; i < BODY_COUNT; ++i) {
			if (sensor.bodyIsidentified(i)) {
				identifiedBody = i;
				break;
			}
		}

		//Left and right must be mirrored

		//todo: fingers
		KinectJoint2CineticoBodyPoint(BodyPoint::Head, JointType_Head);
		KinectJoint2CineticoBodyPoint(BodyPoint::SpineShoulder, JointType_SpineShoulder);
		KinectJoint2CineticoBodyPoint(BodyPoint::Spine, JointType_SpineMid);
		KinectJoint2CineticoBodyPoint(BodyPoint::SpineBase, JointType_SpineBase);
		//Left
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftShoulder, JointType_ShoulderLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftElbow, JointType_ElbowLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftWrist, JointType_WristLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftHandCenter, JointType_HandLeft);
		//fingers
		//KinectJoint2CineticoBodyPoint(BodyPoint::LeftPinky, JointType_HandLeft);
		//KinectJoint2CineticoBodyPoint(BodyPoint::LeftRingFinger, JointType_HandLeft);
		//KinectJoint2CineticoBodyPoint(BodyPoint::LeftMiddleFinger, JointType_HandLeft);
		//KinectJoint2CineticoBodyPoint(BodyPoint::LeftIndexFinger, JointType_HandLeft);
		//KinectJoint2CineticoBodyPoint(BodyPoint::LeftThumb, JointType_HandLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftHip, JointType_HipLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftKnee, JointType_KneeLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftAnkle, JointType_AnkleLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftFoot, JointType_FootLeft);
		//Right
		KinectJoint2CineticoBodyPoint(BodyPoint::RightShoulder, JointType_ShoulderRight);
		KinectJoint2CineticoBodyPoint(BodyPoint::RightElbow, JointType_ElbowRight);
		KinectJoint2CineticoBodyPoint(BodyPoint::RightWrist, JointType_WristRight);
		KinectJoint2CineticoBodyPoint(BodyPoint::RightHandCenter, JointType_HandRight);
		//fingers
		//KinectJoint2CineticoBodyPoint(BodyPoint::RightPinky, JointType_HandRight);
		//KinectJoint2CineticoBodyPoint(BodyPoint::RightRingFinger, JointType_HandRight);
		//KinectJoint2CineticoBodyPoint(BodyPoint::RightMiddleFinger, JointType_HandRight);
		//KinectJoint2CineticoBodyPoint(BodyPoint::RightIndexFinger, JointType_HandRight);
		//KinectJoint2CineticoBodyPoint(BodyPoint::RightThumb, JointType_HandRight);
		KinectJoint2CineticoBodyPoint(BodyPoint::RightHip, JointType_HipRight);
		KinectJoint2CineticoBodyPoint(BodyPoint::RightKnee, JointType_KneeRight);
		KinectJoint2CineticoBodyPoint(BodyPoint::RightAnkle, JointType_AnkleRight);
		KinectJoint2CineticoBodyPoint(BodyPoint::RightFoot, JointType_FootRight);

		m_body->m_identifiedBodyPointCount = identifiedBodyPointCount;
		return true;
#else
		return false;
#endif
	}

}