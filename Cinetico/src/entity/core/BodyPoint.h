#ifndef __CINETICO_CORE_BODYPOINT_H__
#define __CINETICO_CORE_BODYPOINT_H__

#include <string>
#include "vector3.h"

namespace cinetico_core {

	class BodyPoint {
		friend class BodyTracker;
		friend class Body;

	public:
		enum BodyPart
		{
			Head,
			SpineShoulder,
			Spine,
			SpineBase,
			LeftShoulder,
			LeftElbow,
			LeftWrist,
			LeftHandCenter,
			LeftPinky,
			LeftRingFinger,
			LeftMiddleFinger,
			LeftIndexFinger,
			LeftThumb,
			LeftHip,
			LeftKnee,
			LeftAnkle,
			LeftFoot,
			RightShoulder,
			RightElbow,
			RightWrist,
			RightHandCenter,
			RightPinky,
			RightRingFinger,
			RightMiddleFinger,
			RightIndexFinger,
			RightThumb,
			RightHip,
			RightKnee,
			RightAnkle,
			RightFoot,

			BodyPartCount
		};

		Body &m_ownerBody;
		BodyPart m_bodyPart;
		std::string m_tag;
		Vector3 m_position;
		Vector3 m_orientation;
		bool m_trackable;
		bool m_identified;
		bool m_occluded;

	public:
		BodyPoint(Body& ownerBody, BodyPart bodyPart, const char *tag);

		void setTrackable(bool trackable) { m_trackable = trackable; }

		inline const BodyPart bodyPart() const { return m_bodyPart; }
		inline const std::string &tag() const { return m_tag; }
		inline const Vector3 &position() const { return m_position; }
		inline bool trackable() const { return m_trackable; }
		inline bool isIdentified() const { return m_identified; }
		inline bool isOccluded() const { return m_occluded; }
	};

}

#endif