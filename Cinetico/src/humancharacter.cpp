
#include "cineticoui.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"
#include "humancharacter.h"

namespace cinetico {

	using namespace render3d;
	using render3d::Color;

#define CM2W 2

#define HEAD_SIZE (4.f/8.f)
#define FOOT_SIZE HEAD_SIZE/(1.6f)
#define HAND_SIZE HEAD_SIZE/(2.f)
#define ELBOW_SIZE HEAD_SIZE/(6.f)
#define KNEE_SIZE HEAD_SIZE/(6.f)

#define BONE_COLOR Color(255,255,255)

	render3d::Color boneColors[] = 
	{
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
		BONE_COLOR, BONE_COLOR,
	};

	render3d::Vector3 bonePositions[100];
	int vertexCount = 0;

	render3d::Color bodyColors[] =
	{
		//Front
		render3d::Color(255,0,0),
		render3d::Color(255,0,0),
		render3d::Color(255,0,0),
		render3d::Color(255,0,0),

		//Back
		Color(0,255,0),
		Color(0,255,0),
		Color(0,255,0),
		Color(0,255,0),

		//Top
		Color(255,255,255),
		Color(255,255,255),
		Color(255,255,255),
		Color(255,255,255),

		//Bottom
		Color(0,0,0),
		Color(0,0,0),
		Color(0,0,0),
		Color(0,0,0),

		//Left
		Color(0,0,255),
		Color(0,0,255),
		Color(0,0,255),
		Color(0,0,255),

		//Right
		Color(255,0,255),
		Color(255,0,255),
		Color(255,0,255),
		Color(255,0,255),
	};

	struct BodyResourceIds {
		int head;
		int spine;
		int elbow;
		int hand;
		int knee;
		int foot;
	};

	BodyResourceIds g_bodyResourceIds;

	static bool humanBodyModelLoaded = false;

	int mapBones(int index, Body *body, const cinetico_core::Vector3 &position, BodyPoint::BodyPart from, BodyPoint::BodyPart to) {
		cinetico_core::Vector3 posFrom = body->bodyPoint(from)->position() * CM2W + position;
		cinetico_core::Vector3 posTo = body->bodyPoint(to)->position() * CM2W + position;
		bonePositions[index++] = render3d::Vector3(posFrom.x(), posFrom.y(), posFrom.z());
		bonePositions[index++] = render3d::Vector3(posTo.x(), posTo.y(), posTo.z());
		return index;
	}

	HumanCharacter::HumanCharacter(CineticoUI &cineticoUI)
		: Character(cineticoUI) {

		if (!humanBodyModelLoaded) {

			RenderEngine *renderEngine = m_cineticoUI.renderEngine();
			RenderEngineHelper *renderEngineHelper = m_cineticoUI.renderEngineHelper();

			BodyResourceIds &resId = g_bodyResourceIds;
			resId.head = renderEngineHelper->createCube(HEAD_SIZE);
			renderEngine->resourceData(resId.head)->setColors(bodyColors);
			resId.spine = renderEngineHelper->createRectangularPrism(HEAD_SIZE / 3.5f, HEAD_SIZE * 1.5, HEAD_SIZE / 3.5f);
			renderEngine->resourceData(resId.spine)->setColors(bodyColors);
			resId.elbow = renderEngineHelper->createCube(ELBOW_SIZE);
			renderEngine->resourceData(resId.elbow)->setColors(bodyColors);
			resId.hand = renderEngineHelper->createRectangularPrism(HAND_SIZE / 10.f,HAND_SIZE/1.3f,HAND_SIZE);
			renderEngine->resourceData(resId.hand)->setColors(bodyColors);
			resId.knee = renderEngineHelper->createCube(KNEE_SIZE);
			renderEngine->resourceData(resId.knee)->setColors(bodyColors);
			resId.foot = renderEngineHelper->createRectangularPrism(FOOT_SIZE / 1.3f, FOOT_SIZE / 10.f , FOOT_SIZE);
			renderEngine->resourceData(resId.foot)->setColors(bodyColors);

			humanBodyModelLoaded = true;
		}

		BodyResourceIds &resId = g_bodyResourceIds;
		RenderEngine *renderEngine = m_cineticoUI.renderEngine();

		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.head));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.spine));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.elbow));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.elbow));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.hand));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.hand));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.knee));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.knee));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.foot));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.foot));
	}

	void HumanCharacter::mapBodyPointNodeToWorldPoint(int instId, BodyPoint::BodyPart bodyPoint) {
		ResourceInstance *instance = m_cineticoUI.renderEngine()->resourceInstance(instId);
		cinetico_core::Vector3 pos = m_bodyPoints[bodyPoint]->globalPosition() * CM2W;
		BodyPointNode *parent = m_bodyPoints[bodyPoint]->parent();
		cinetico_core::Quaternion rot;
		if(parent)
			rot = m_bodyPoints[bodyPoint]->globalOrientation();
		rot = m_bodyPoints[bodyPoint]->globalOrientation();
		cinetico_core::Vector3 euler = rot.toEuler();
		instance->setPos(render3d::Vector3(pos.x(), pos.y(), pos.z()));
		instance->setRotation(rot);
	}

	inline void HumanCharacter::mapBodyPointToCharacterBodyPointNode(BodyPoint::BodyPart bodyPoint) {
		cinetico_core::Vector3 pos = m_body->bodyPoint(bodyPoint)->position();
		cinetico_core::Quaternion rot = m_body->bodyPoint(bodyPoint)->orientation();
		m_bodyPoints[bodyPoint]->setGlobalPosition(pos);
		m_bodyPoints[bodyPoint]->setLocalOrientation(rot);
	}

	void HumanCharacter::update() {
		m_body = m_cineticoUI.cinetico().bodyTracker()->body();
		if (m_body) {
			//cabeça até cervical
			int index = 0;

			for (int i = 0; i < BodyPoint::BodyPartCount; ++i) {
				mapBodyPointToCharacterBodyPointNode((BodyPoint::BodyPart)i);
			}

			index = mapBones(index, m_body, m_position, BodyPoint::Head, BodyPoint::Cervical);
			index = mapBones(index, m_body, m_position, BodyPoint::Cervical, BodyPoint::LeftShoulder);
			index = mapBones(index, m_body, m_position, BodyPoint::LeftShoulder, BodyPoint::LeftElbow);
			index = mapBones(index, m_body, m_position, BodyPoint::LeftElbow, BodyPoint::LeftPalm);
			index = mapBones(index, m_body, m_position, BodyPoint::Cervical, BodyPoint::RightShoulder);
			index = mapBones(index, m_body, m_position, BodyPoint::RightShoulder, BodyPoint::RightElbow);
			index = mapBones(index, m_body, m_position, BodyPoint::RightElbow, BodyPoint::RightPalm);
			index = mapBones(index, m_body, m_position, BodyPoint::Cervical, BodyPoint::Spine);
			index = mapBones(index, m_body, m_position, BodyPoint::Spine, BodyPoint::Pelvis);
			index = mapBones(index, m_body, m_position, BodyPoint::Pelvis, BodyPoint::LeftHip);
			index = mapBones(index, m_body, m_position, BodyPoint::LeftHip, BodyPoint::LeftKnee);
			index = mapBones(index, m_body, m_position, BodyPoint::LeftKnee, BodyPoint::LeftAnkle);
			index = mapBones(index, m_body, m_position, BodyPoint::LeftAnkle, BodyPoint::LeftFoot);
			index = mapBones(index, m_body, m_position, BodyPoint::Pelvis, BodyPoint::RightHip);
			index = mapBones(index, m_body, m_position, BodyPoint::RightHip, BodyPoint::RightKnee);
			index = mapBones(index, m_body, m_position, BodyPoint::RightKnee, BodyPoint::RightAnkle);
			index = mapBones(index, m_body, m_position, BodyPoint::RightAnkle, BodyPoint::RightFoot);
			vertexCount = index;
		}
		Character::update();
		mapBodyPointNodeToWorldPoint(m_instanceIds[0], BodyPoint::Head);
		mapBodyPointNodeToWorldPoint(m_instanceIds[1], BodyPoint::Spine);
		mapBodyPointNodeToWorldPoint(m_instanceIds[2], BodyPoint::LeftElbow);
		mapBodyPointNodeToWorldPoint(m_instanceIds[3], BodyPoint::RightElbow);
		mapBodyPointNodeToWorldPoint(m_instanceIds[4], BodyPoint::LeftPalm);
		mapBodyPointNodeToWorldPoint(m_instanceIds[5], BodyPoint::RightPalm);
		mapBodyPointNodeToWorldPoint(m_instanceIds[6], BodyPoint::LeftKnee);
		mapBodyPointNodeToWorldPoint(m_instanceIds[7], BodyPoint::RightKnee);
		mapBodyPointNodeToWorldPoint(m_instanceIds[8], BodyPoint::LeftFoot);
		mapBodyPointNodeToWorldPoint(m_instanceIds[9], BodyPoint::RightFoot);
	}

	void HumanCharacter::render() {
		if (!m_body || m_body->identifiedBodyPointCount() == 0) return;
		RenderEngine *renderEngine = m_cineticoUI.renderEngine();
		//renderEngine->drawResourceDirect(bonePositions,vertexCount,boneColors);
		
		for (unsigned int i = 0; i < m_instanceIds.size(); ++i) {
			renderEngine->drawResource(m_instanceIds[i]);
			render3d::Vector3 line[2];
			ResourceInstance *inst = renderEngine->resourceInstance(m_instanceIds[i]);
			cinetico_core::Vector3 rot = inst->rot().toEuler();
			line[0].set(inst->pos().x(), inst->pos().y(), inst->pos().z());
			float lineSize = 0.3f;
			line[1].set(inst->pos().x() + rot.x()*lineSize, inst->pos().y() + rot.y()*lineSize, inst->pos().z() + rot.z()*lineSize);
			renderEngine->drawResourceDirect(line, 6, boneColors);
		}
		
		/*
		ResourceInstance *inst = renderEngine->resourceInstance(g_bodyResourceIds.spine);
		renderEngine->drawResource(g_bodyResourceIds.spine);
		render3d::Vector3 line[2];
		cinetico_core::Vector3 rot = inst->rot().toEuler();
		line[0].set(inst->pos().x(), inst->pos().y(), inst->pos().z());
		float lineSize = 1.f;
		line[1].set(inst->pos().x() + rot.x()*lineSize, inst->pos().y() + rot.y()*lineSize, inst->pos().z() + rot.z()*lineSize);
		renderEngine->drawResourceDirect(line, 6, boneColors);
		*/
	}
}
