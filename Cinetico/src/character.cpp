
#include "character.h"

namespace cinetico {

	void Character::createBoneHierarchyTree() {
		BodyPointNode *spineBase = new BodyPointNode(BodyPoint::SpineBase, NULL);
		
		BodyPointNode *spine = new BodyPointNode(BodyPoint::Spine, spineBase);
		BodyPointNode *cervical = new BodyPointNode(BodyPoint::Cervical, spine);
		BodyPointNode *head = new BodyPointNode(BodyPoint::Head, cervical);

		BodyPointNode *leftHip = new BodyPointNode(BodyPoint::LeftHip, spineBase);
		BodyPointNode *leftKnee = new BodyPointNode(BodyPoint::LeftKnee, leftHip);
		BodyPointNode *leftAnkle = new BodyPointNode(BodyPoint::LeftAnkle, leftKnee);
		BodyPointNode *leftFoot = new BodyPointNode(BodyPoint::LeftFoot, leftAnkle);

		BodyPointNode *rightHip = new BodyPointNode(BodyPoint::RightHip, spineBase);
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
	}

	Character::Character(CineticoUI &cineticoUI)
		: m_cineticoUI(cineticoUI) {

	}

}