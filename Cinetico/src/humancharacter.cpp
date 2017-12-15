
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

	int resHadouken;
	int instHadouken;

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

		std::vector<int> res = renderEngineHelper->loadModel("resources/ball.dae");
		resHadouken = res[0];

		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.head));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.spine));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.elbow));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.elbow));
		//renderEngine->resourceInstance(m_instanceIds.back())->setScale(render3d::Vector3(-1, -1, -1));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.hand));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.hand));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.knee));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.knee));
		//renderEngine->resourceInstance(m_instanceIds.back())->setScale(render3d::Vector3(-1, -1, -1));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.foot));
		m_instanceIds.push_back(renderEngine->newResourceInstance(resId.foot));
		//renderEngine->resourceInstance(m_instanceIds.back())->setScale(render3d::Vector3(-1, -1, -1));

		instHadouken = renderEngine->newResourceInstance(resHadouken);
		renderEngine->resourceInstance(instHadouken)->setScale(0.25f);
	}

	void HumanCharacter::mapBodyPointNodeToWorldPoint(int instId, BodyPoint::BodyPart bodyPoint, bool mirrored) {
		ResourceInstance *instance = m_cineticoUI.renderEngine()->resourceInstance(instId);
		cinetico_core::Vector3 pos = m_bodyPoints[bodyPoint]->globalPosition() * CM2W;
		BodyPointNode *parent = m_bodyPoints[bodyPoint]->parent();
		Quaternion rot = m_bodyPoints[bodyPoint]->globalOrientation();
		//rot = Quaternion::fromRotationMatrix(m_bodyPoints[bodyPoint]->globalTransform());
		cinetico_core::Vector3 euler = rot.toEuler();
		instance->setPos(render3d::Vector3(pos.x(), pos.y(), pos.z()));
		instance->setRotation(rot);
	}

	inline void HumanCharacter::mapBodyPointToCharacterBodyPointNode(BodyPoint::BodyPart bodyPoint) {
		cinetico_core::Vector3 pos = m_body->bodyPoint(bodyPoint)->position();
		cinetico_core::Quaternion rot = m_body->bodyPoint(bodyPoint)->orientation();
		m_bodyPoints[bodyPoint]->setGlobalPosition(pos);
		BodyPointNode *node = m_bodyPoints[bodyPoint];
		BodyPointNode *parent = m_bodyPoints[bodyPoint]->parent();
		if (parent) {
			Quaternion qt1 = rot*m_body->bodyPoint(parent->bp())->orientation().inverse();
			Quaternion qt2 = m_body->bodyPoint(parent->bp())->orientation()*qt1;
			node->setLocalOrientation(rot*m_body->bodyPoint(parent->bp())->orientation().inverse());
		}
		else
			node->setLocalOrientation(rot);
		m_bodyPoints[bodyPoint]->setGlobalOrientation(rot);
	}

	void HumanCharacter::update() {
		m_body = m_cineticoUI.cinetico().bodyTracker()->body();
		if (m_body) {
			//cabeça até cervical
			int index = 0;

			for (int i = 0; i < BodyPoint::BodyPartCount; ++i) {
				mapBodyPointToCharacterBodyPointNode((BodyPoint::BodyPart)i);
			}

			index = mapBones(index, m_body, m_position, BodyPoint::Head, BodyPoint::Neck);
			index = mapBones(index, m_body, m_position, BodyPoint::Neck, BodyPoint::Cervical);
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
		//Character::update();
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


	static cinetico_core::Vector3 getRot(Body *body, BodyPoint::BodyPart bp) {
		return body->bodyPoint(bp)->orientation().rotatedVector(cinetico_core::Vector3(0,1,0));
	}

	static void drawLine(RenderEngine* renderEngine, Body *body, BodyPoint::BodyPart bp) {
		render3d::Vector3 line[2];
		cinetico_core::Vector3 pos = body->bodyPoint(bp)->position()*CM2W;
		cinetico_core::Vector3 dir = getRot(body, bp);
		//cinetico_core::Vector3 rot = inst->rot().toEuler();
		line[0].set(pos.x(), pos.y(), pos.z());
		float lineSize = 1.f;
		line[1].set(pos.x() + dir.x()*lineSize, pos.y() + dir.y()*lineSize, pos.z() + dir.z()*lineSize);
		renderEngine->drawResourceDirect(line, 2, boneColors);
	}

	static void drawLine2(RenderEngine* renderEngine, const cinetico_core::Vector3 &pos, const cinetico_core::Vector3 &dir) {
		render3d::Vector3 line[2];
		//cinetico_core::Vector3 rot = inst->rot().toEuler();
		line[0].set(pos.x(), pos.y(), pos.z());
		float lineSize = 1.f;
		line[1].set(pos.x() + dir.x()*lineSize, pos.y() + dir.y()*lineSize, pos.z() + dir.z()*lineSize);
		renderEngine->drawResourceDirect(line, 2, boneColors);
	}

	void HumanCharacter::render() {
		if (!m_body || m_body->identifiedBodyPointCount() == 0) return;
		RenderEngine *renderEngine = m_cineticoUI.renderEngine();
		renderEngine->drawResourceDirect(bonePositions,vertexCount,boneColors);
		
		for (unsigned int i = 0; i < m_instanceIds.size(); ++i) {
			renderEngine->drawResource(m_instanceIds[i]);
			
		}

		drawLine(renderEngine, m_body, BodyPoint::LeftElbow);
		//drawLine(renderEngine, m_body, BodyPoint::RightElbow);
		//drawLine(renderEngine, m_body, BodyPoint::Pelvis);
		drawLine2(renderEngine, m_body->bodyPoint(BodyPoint::RightElbow)->position()*CM2W, m_body->bodyPoint(BodyPoint::RightElbow)->orientation().rotatedVector(cinetico_core::Vector3(0, 0, -1)));
		drawLine2(renderEngine, m_body->bodyPoint(BodyPoint::Pelvis)->position()*CM2W, m_body->bodyPoint(BodyPoint::Pelvis)->orientation().rotatedVector(cinetico_core::Vector3(0, 0, -1)));

		cinetico_core::Vector3 lElbowRot = getRot(m_body, BodyPoint::LeftElbow);
		cinetico_core::Vector3 rElbowRot = getRot(m_body, BodyPoint::RightElbow);
		cinetico_core::Vector3 pelvisRot = getRot(m_body, BodyPoint::Pelvis);
		cinetico_core::Vector3 pelvisBinormal = m_body->bodyPoint(BodyPoint::Pelvis)->orientation().rotatedVector(cinetico_core::Vector3(0, 0, -1));
		float dot = dotProduct(lElbowRot, rElbowRot);
		float dot2 = dotProduct(lElbowRot, pelvisBinormal);
		static int cond = 0;
		static int cond2 = 0;
		static render3d::Vector3 hadoPos;
		static render3d::Vector3 hadoDir;
		string str = "Dot: ";
		str += string::fromFloat(dot);

		if (cond == 0 && dot >= -0.3f && dot <= 0.3f && dot2 >= 0.8f)
			++cond;
		if (cond == 1 && dot >= 0.9f)
			++cond;
		if (dot <= -0.8f) {
			cond = 0;
			cond2 = 0;
		}
		if (cond == 2) {
			if (cond2 == 0) {
				cinetico_core::Vector3 newp = (m_body->bodyPoint(BodyPoint::LeftElbow)->position() + m_body->bodyPoint(BodyPoint::RightElbow)->position()) / 2 * CM2W;
				cinetico_core::Vector3 newd = (lElbowRot + rElbowRot / 2) * CM2W;
				hadoPos = render3d::Vector3(newp.x(), newp.y(), newp.z());
				hadoDir = render3d::Vector3(newd.x(), newd.y(), newd.z());
				renderEngine->resourceInstance(instHadouken)->setPos(hadoPos);
				++cond2;
			}
			else if (cond2 == 1) {
				renderEngine->resourceInstance(instHadouken)->setPos(renderEngine->resourceInstance(instHadouken)->pos() + hadoDir*0.03f);
			}
			str = "Hadouken";
		}
		str += "(" + string::fromInteger(cond) + ")";
		renderEngine->drawText(str.data(), 10, 0, render3d::Color(255, 255, 255));

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
		renderEngine->drawResource(instHadouken);
	}
}
