

#include "Body.h"
#include "bodytracker.h"

namespace cinetico_core {

	Body::Body(Sensor &sensor) :
		m_sensor(sensor)
	{
		struct BodyPartTag {
			BodyPoint::BodyPart bodyPart;
			const char *tag;
		};

		BodyPartTag bodyPartTag[] = {
			{ BodyPoint::Head, "Head" },
			{ BodyPoint::SpineShoulder, "Spine Shoulder" },
			{ BodyPoint::Spine, "Spine" },
			{ BodyPoint::SpineBase, "Spine Base" },
			{ BodyPoint::LeftShoulder, "Left Shoulder" },
			{ BodyPoint::LeftElbow, "Left Elbow" },
			{ BodyPoint::LeftWrist, "Left Wrist" },
			{ BodyPoint::LeftHandCenter, "Left Palm" },
			{ BodyPoint::RightShoulder, "Right Shoulder" },
			{ BodyPoint::RightElbow, "Right Elbow" },
			{ BodyPoint::RightWrist, "Right Wrist" },
			{ BodyPoint::RightHandCenter, "Right Palm" },
			{ BodyPoint::LeftFoot, "Left Foot" },
		};


		m_identifiedBodyPointCount = 0;
		int i;

		for (i = 0; i < BodyPoint::BodyPartCount; ++i) {
			m_bodyPoint[i] = NULL;
		}

		for(i = 0; i < sizeof(bodyPartTag) / sizeof(bodyPartTag[0]); ++i) {
			m_bodyPoint[i] = new BodyPoint(*this, bodyPartTag[i].bodyPart, bodyPartTag[i].tag);
		}

		for (i = 0; i < BodyPoint::BodyPartCount; ++i) {
			if (!m_bodyPoint[i])
				m_bodyPoint[i] = new BodyPoint(*this, (BodyPoint::BodyPart)i, "UNMAPPED");
		}
	}

	void Body::setBodyPointsTrackable(unsigned long bodyParts, bool trackable) {
		//todo
	}

	void Body::setAllBodyPointsTrackable(bool trackable) {
		for (unsigned int i = 0; i < BodyPoint::BodyPartCount; ++i) {
			m_bodyPoint[i]->setTrackable(trackable);
		}
	}

}