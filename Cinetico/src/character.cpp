
#include "character.h"

namespace cinetico {

	BodyPointNode *Character::addBodyPointNode(BodyPoint::BodyPart bp, BodyPointNode *parent) {
		BodyPointNode *bpn = new BodyPointNode(bp, parent);
		m_bodyPoints[bp] = bpn;
		return bpn;
	}

	void Character::createBoneHierarchyTree() {
		m_bodyPoints.resize(BodyPoint::BodyPartCount);
		BodyPointNode *pelvis = addBodyPointNode(BodyPoint::Pelvis, NULL);
		
		BodyPointNode *spine = addBodyPointNode(BodyPoint::Spine, pelvis);
		BodyPointNode *cervical = addBodyPointNode(BodyPoint::Cervical, spine);
		BodyPointNode *neck = addBodyPointNode(BodyPoint::Neck, cervical);
		BodyPointNode *head = addBodyPointNode(BodyPoint::Head, neck);

		BodyPointNode *leftHip = addBodyPointNode(BodyPoint::LeftHip, pelvis);
		BodyPointNode *leftKnee = addBodyPointNode(BodyPoint::LeftKnee, leftHip);
		BodyPointNode *leftAnkle = addBodyPointNode(BodyPoint::LeftAnkle, leftKnee);
		BodyPointNode *leftFoot = addBodyPointNode(BodyPoint::LeftFoot, leftAnkle);

		BodyPointNode *rightHip = addBodyPointNode(BodyPoint::RightHip, pelvis);
		BodyPointNode *rightKnee = addBodyPointNode(BodyPoint::RightKnee, rightHip);
		BodyPointNode *rightAnkle = addBodyPointNode(BodyPoint::RightAnkle, rightKnee);
		BodyPointNode *rightFoot = addBodyPointNode(BodyPoint::RightFoot, rightAnkle);

		BodyPointNode *leftShoulder = addBodyPointNode(BodyPoint::LeftShoulder, cervical);
		BodyPointNode *leftElbow = addBodyPointNode(BodyPoint::LeftElbow, leftShoulder);
		BodyPointNode *leftWrist = addBodyPointNode(BodyPoint::LeftWrist, leftElbow);
		BodyPointNode *leftPalm = addBodyPointNode(BodyPoint::LeftPalm, leftWrist);
		BodyPointNode *leftPinky = addBodyPointNode(BodyPoint::LeftPinky, leftPalm);
		BodyPointNode *leftRingFinger = addBodyPointNode(BodyPoint::LeftRingFinger, leftPalm);
		BodyPointNode *leftMiddleFinger = addBodyPointNode(BodyPoint::LeftMiddleFinger, leftPalm);
		BodyPointNode *leftIndexFinger = addBodyPointNode(BodyPoint::LeftIndexFinger, leftPalm);
		BodyPointNode *leftThumb = addBodyPointNode(BodyPoint::LeftThumb, leftPalm);

		BodyPointNode *rightShoulder = addBodyPointNode(BodyPoint::RightShoulder, cervical);
		BodyPointNode *rightElbow = addBodyPointNode(BodyPoint::RightElbow, rightShoulder);
		BodyPointNode *rightWrist = addBodyPointNode(BodyPoint::RightWrist, rightElbow);
		BodyPointNode *rightPalm = addBodyPointNode(BodyPoint::RightPalm, rightWrist);
		BodyPointNode *rightPinky = addBodyPointNode(BodyPoint::RightPinky, rightPalm);
		BodyPointNode *rightRingFinger = addBodyPointNode(BodyPoint::RightRingFinger, rightPalm);
		BodyPointNode *rightMiddleFinger = addBodyPointNode(BodyPoint::RightMiddleFinger, rightPalm);
		BodyPointNode *rightIndexFinger = addBodyPointNode(BodyPoint::RightIndexFinger, rightPalm);
		BodyPointNode *rightThumb = addBodyPointNode(BodyPoint::RightThumb, rightPalm);

		m_rootBodyPoint = pelvis;
	}

	Character::Character(CineticoUI &cineticoUI)
		: m_cineticoUI(cineticoUI) {
		createBoneHierarchyTree();
	}

	void Character::update() {
		Matrix4x4 identityTransform = Quaternion().toRotationMatrix();
		m_rootBodyPoint->updateState(Quaternion(),identityTransform);
	}

}