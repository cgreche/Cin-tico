
#include "bodytracker.h"
#ifdef _WIN32
#include "kinectsensor.h"
#include "entity/core/quaternion.h"
#endif

namespace cinetico_core {

	BodyTracker::BodyTracker(Sensor &sensor)
		: m_sensor(sensor) {
		m_trackableBodyPoints = 0;
		m_identifiedBodyCount = 0;
		m_body = NULL;
	}

	BodyTracker::~BodyTracker() {
		if (m_body)
			delete m_body;
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
		Vector3 position = Vector3(joint.Position.X, joint.Position.Y, joint.Position.Z); \
		Quaternion quat(jointOrientation.Orientation.x,jointOrientation.Orientation.y,jointOrientation.Orientation.z,jointOrientation.Orientation.w); \
		Vector3 orientation = quat.toEuler(); \
		if(p->trackable()) { \
			p->m_position = position; \
			p->m_orientation = orientation; \
			p->m_identified = joint.TrackingState != TrackingState_NotTracked; \
			p->m_occluded = joint.TrackingState != TrackingState_Tracked; \
			if(p->m_identified) identifiedBodyPointCount++; \
		} \
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

		if (!m_body) {
			m_body = new Body(sensor);
			if (m_trackableBodyPoints == 0)
				m_body->setAllBodyPointsTrackable(true);
			else {
				m_body->setAllBodyPointsTrackable(false);

				if (m_trackableBodyPoints & (1 << 0)) m_body->setBodyPointTrackable(BodyPoint::Head, true);
				if (m_trackableBodyPoints & (1 << 1)) m_body->setBodyPointTrackable(BodyPoint::Cervical, true);
				if (m_trackableBodyPoints & (1 << 2)) m_body->setBodyPointTrackable(BodyPoint::Spine, true);
				if (m_trackableBodyPoints & (1 << 3)) m_body->setBodyPointTrackable(BodyPoint::SpineBase, true);
				if (m_trackableBodyPoints & (1 << 4)) m_body->setBodyPointTrackable(BodyPoint::LeftShoulder, true);
				if (m_trackableBodyPoints & (1 << 5)) m_body->setBodyPointTrackable(BodyPoint::LeftElbow, true);
				if (m_trackableBodyPoints & (1 << 6)) m_body->setBodyPointTrackable(BodyPoint::LeftWrist, true);
				if (m_trackableBodyPoints & (1 << 7)) m_body->setBodyPointTrackable(BodyPoint::LeftPalm, true);
				if (m_trackableBodyPoints & (1 << 8)) m_body->setBodyPointTrackable(BodyPoint::LeftPinky, true);
				if (m_trackableBodyPoints & (1 << 9)) m_body->setBodyPointTrackable(BodyPoint::LeftRingFinger, true);
				if (m_trackableBodyPoints & (1 << 10)) m_body->setBodyPointTrackable(BodyPoint::LeftMiddleFinger, true);
				if (m_trackableBodyPoints & (1 << 11)) m_body->setBodyPointTrackable(BodyPoint::LeftIndexFinger, true);
				if (m_trackableBodyPoints & (1 << 12)) m_body->setBodyPointTrackable(BodyPoint::LeftThumb, true);
				if (m_trackableBodyPoints & (1 << 13)) m_body->setBodyPointTrackable(BodyPoint::LeftHip, true);
				if (m_trackableBodyPoints & (1 << 14)) m_body->setBodyPointTrackable(BodyPoint::LeftKnee, true);
				if (m_trackableBodyPoints & (1 << 15)) m_body->setBodyPointTrackable(BodyPoint::LeftAnkle, true);
				if (m_trackableBodyPoints & (1 << 16)) m_body->setBodyPointTrackable(BodyPoint::LeftFoot, true);
				if (m_trackableBodyPoints & (1 << 17)) m_body->setBodyPointTrackable(BodyPoint::RightShoulder, true);
				if (m_trackableBodyPoints & (1 << 18)) m_body->setBodyPointTrackable(BodyPoint::RightElbow, true);
				if (m_trackableBodyPoints & (1 << 19)) m_body->setBodyPointTrackable(BodyPoint::RightWrist, true);
				if (m_trackableBodyPoints & (1 << 20)) m_body->setBodyPointTrackable(BodyPoint::RightPalm, true);
				if (m_trackableBodyPoints & (1 << 21)) m_body->setBodyPointTrackable(BodyPoint::RightPinky, true);
				if (m_trackableBodyPoints & (1 << 22)) m_body->setBodyPointTrackable(BodyPoint::RightRingFinger, true);
				if (m_trackableBodyPoints & (1 << 23)) m_body->setBodyPointTrackable(BodyPoint::RightMiddleFinger, true);
				if (m_trackableBodyPoints & (1 << 24)) m_body->setBodyPointTrackable(BodyPoint::RightIndexFinger, true);
				if (m_trackableBodyPoints & (1 << 25)) m_body->setBodyPointTrackable(BodyPoint::RightThumb, true);
				if (m_trackableBodyPoints & (1 << 26)) m_body->setBodyPointTrackable(BodyPoint::RightHip, true);
				if (m_trackableBodyPoints & (1 << 27)) m_body->setBodyPointTrackable(BodyPoint::RightKnee, true);
				if (m_trackableBodyPoints & (1 << 28)) m_body->setBodyPointTrackable(BodyPoint::RightAnkle, true);
				if (m_trackableBodyPoints & (1 << 29)) m_body->setBodyPointTrackable(BodyPoint::RightFoot, true);
			}
		}

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
		KinectJoint2CineticoBodyPoint(BodyPoint::Cervical, JointType_SpineShoulder);
		KinectJoint2CineticoBodyPoint(BodyPoint::Spine, JointType_SpineMid);
		KinectJoint2CineticoBodyPoint(BodyPoint::SpineBase, JointType_SpineBase);
		//Left
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftShoulder, JointType_ShoulderLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftElbow, JointType_ElbowLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftWrist, JointType_WristLeft);
		KinectJoint2CineticoBodyPoint(BodyPoint::LeftPalm, JointType_HandLeft);
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
		KinectJoint2CineticoBodyPoint(BodyPoint::RightPalm, JointType_HandRight);
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