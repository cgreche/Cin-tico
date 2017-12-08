#include "bodytracker.h"
#ifdef _WIN32
#include "kinectsensor.h"
#include "core/lib/quaternion.h"
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

#ifdef _WIN32
	void KinectJoint2CineticoBodyPoint(KinectSensor &sensor, Body *body, BodyPoint::BodyPart bpid, int bodyIndex, JointType kj) {
		BodyPoint *bp = body->bodyPoint(bpid);
		if (bp->trackable()) {
			Joint joint = sensor.joint(bodyIndex, kj);
			JointOrientation jointOrientation = sensor.jointOrientation(bodyIndex, kj);
			Vector3 position = Vector3(joint.Position.X, joint.Position.Y, joint.Position.Z);
			Quaternion orientation(jointOrientation.Orientation.w, jointOrientation.Orientation.x, jointOrientation.Orientation.y, jointOrientation.Orientation.z);
			orientation *= Quaternion(0, 0, 1, 0);
			bp->m_position = position;
			bp->m_orientation = orientation;
			bp->m_identified = joint.TrackingState != TrackingState_NotTracked;
			bp->m_occluded = joint.TrackingState != TrackingState_Tracked;
		}
	}
#endif

	bool BodyTracker::track() {
		KinectSensor &sensor = (KinectSensor&)m_sensor;
		sensor.update();

		m_identifiedBodyCount = sensor.identifiedBodyCount();
		if (m_identifiedBodyCount != 1) {
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

				for (int i = 0; i < BodyPoint::BodyPartCount; ++i)
					m_body->setBodyPointTrackable((BodyPoint::BodyPart)i, m_trackableBodyPoints & (1 << i));
			}
		}

		int identifiedBody;
		for (int i = 0; i < BODY_COUNT; ++i) {
			if (sensor.bodyIsidentified(i)) {
				identifiedBody = i;
				break;
			}
		}

		//Left and right must be mirrored
#ifdef _WIN32
		//todo: fingers
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::Head, identifiedBody, JointType_Head);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::Neck, identifiedBody, JointType_Neck);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::Cervical, identifiedBody, JointType_SpineShoulder);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::Spine, identifiedBody, JointType_SpineMid);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::Pelvis, identifiedBody, JointType_SpineBase);
		//Left
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftShoulder, identifiedBody, JointType_ShoulderLeft);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftElbow, identifiedBody, JointType_ElbowLeft);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftWrist, identifiedBody, JointType_WristLeft);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftPalm, identifiedBody, JointType_HandLeft);
		//fingers
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftPinky, identifiedBody, JointType_HandLeft);
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftRingFinger, identifiedBody, JointType_HandLeft);
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftMiddleFinger, identifiedBody, JointType_HandLeft);
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftIndexFinger, identifiedBody, JointType_HandLeft);
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftThumb, identifiedBody, JointType_HandLeft);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftHip, identifiedBody, JointType_HipLeft);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftKnee, identifiedBody, JointType_KneeLeft);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftAnkle, identifiedBody, JointType_AnkleLeft);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::LeftFoot, identifiedBody, JointType_FootLeft);
		//Right
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightShoulder, identifiedBody, JointType_ShoulderRight);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightElbow, identifiedBody, JointType_ElbowRight);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightWrist, identifiedBody, JointType_WristRight);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightPalm, identifiedBody, JointType_HandRight);
		//fingers
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightPinky, identifiedBody, JointType_HandRight);
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightRingFinger, identifiedBody, JointType_HandRight);
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightMiddleFinger, identifiedBody, JointType_HandRight);
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightIndexFinger, identifiedBody, JointType_HandRight);
		//KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightThumb, identifiedBody, JointType_HandRight);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightHip, identifiedBody, JointType_HipRight);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightKnee, identifiedBody, JointType_KneeRight);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightAnkle, identifiedBody, JointType_AnkleRight);
		KinectJoint2CineticoBodyPoint((KinectSensor&)m_sensor, m_body, BodyPoint::RightFoot, identifiedBody, JointType_FootRight);

		int identifiedBodyPointCount = 0;
		for (int i = 0; i < BodyPoint::BodyPartCount; ++i) {
			BodyPoint *bp = m_body->bodyPoint((BodyPoint::BodyPart)i);
			if(bp->isIdentified())
				++identifiedBodyPointCount;
		}
		m_body->m_identifiedBodyPointCount = identifiedBodyPointCount;
		return true;
#else
		return false;
#endif
	}

}