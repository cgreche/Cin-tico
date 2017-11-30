
#include "character.h"

namespace cinetico {

	void Character::createBoneHierarchyTree() {
		BodyPointNode *pelvis = new BodyPointNode(BodyPoint::SpineBase, NULL);
		
		BodyPointNode *spine = new BodyPointNode(BodyPoint::Spine, pelvis);
		BodyPointNode *cervical = new BodyPointNode(BodyPoint::Cervical, spine);
		BodyPointNode *head = new BodyPointNode(BodyPoint::Head, cervical);

		BodyPointNode *leftHip = new BodyPointNode(BodyPoint::LeftHip, pelvis);
		BodyPointNode *leftKnee = new BodyPointNode(BodyPoint::LeftKnee, leftHip);
		BodyPointNode *leftAnkle = new BodyPointNode(BodyPoint::LeftAnkle, leftKnee);
		BodyPointNode *leftFoot = new BodyPointNode(BodyPoint::LeftFoot, leftAnkle);

		BodyPointNode *rightHip = new BodyPointNode(BodyPoint::RightHip, pelvis);
		BodyPointNode *rightKnee = new BodyPointNode(BodyPoint::RightKnee, rightHip);
		BodyPointNode *rightAnkle = new BodyPointNode(BodyPoint::RightAnkle, rightKnee);
		BodyPointNode *rightFoot = new BodyPointNode(BodyPoint::RightFoot, rightAnkle);

		BodyPointNode *leftShoulder = new BodyPointNode(BodyPoint::LeftShoulder, cervical);
		BodyPointNode *leftElbow = new BodyPointNode(BodyPoint::LeftElbow, leftShoulder);
		BodyPointNode *leftWrist = new BodyPointNode(BodyPoint::LeftWrist, leftElbow);
		BodyPointNode *leftPalm = new BodyPointNode(BodyPoint::LeftPalm, leftWrist);

		BodyPointNode *rightShoulder = new BodyPointNode(BodyPoint::RightShoulder, cervical);
		BodyPointNode *rightElbow = new BodyPointNode(BodyPoint::RightElbow, rightShoulder);
		BodyPointNode *rightWrist = new BodyPointNode(BodyPoint::RightWrist, rightElbow);
		BodyPointNode *rightPalm = new BodyPointNode(BodyPoint::RightPalm, rightWrist);

		m_bodyPoints.push_back(pelvis);

		m_bodyPoints.push_back(spine);
		m_bodyPoints.push_back(cervical);
		m_bodyPoints.push_back(head);

		m_bodyPoints.push_back(leftHip);
		m_bodyPoints.push_back(leftKnee);
		m_bodyPoints.push_back(leftAnkle);
		m_bodyPoints.push_back(leftFoot);

		m_bodyPoints.push_back(rightHip);
		m_bodyPoints.push_back(rightKnee);
		m_bodyPoints.push_back(rightAnkle);
		m_bodyPoints.push_back(rightFoot);

		m_bodyPoints.push_back(leftShoulder);
		m_bodyPoints.push_back(leftElbow);
		m_bodyPoints.push_back(leftWrist);
		m_bodyPoints.push_back(leftPalm);

		m_bodyPoints.push_back(rightShoulder);
		m_bodyPoints.push_back(rightElbow);
		m_bodyPoints.push_back(rightWrist);
		m_bodyPoints.push_back(rightPalm);

		m_rootBodyPoint = pelvis;
	}

	Character::Character(CineticoUI &cineticoUI)
		: m_cineticoUI(cineticoUI) {
		createBoneHierarchyTree();
	}

	void Character::update() {
		m_rootBodyPoint->updateState(Quaternion());
	}

}