
#include "cineticoui.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"
#include "humancharacter.h"

namespace cinetico {

	using namespace render3d;
	using render3d::Color;

#define CM2W 2

#define HEAD_SIZE (4.f/8.f)
#define FOOT_SIZE HEAD_SIZE/(2.f)
#define HAND_SIZE HEAD_SIZE/(2.f)
#define ELBOW_SIZE HEAD_SIZE/(6.f)
#define KNEE_SIZE HEAD_SIZE/(6.f)

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
			resId.hand = renderEngineHelper->createCube(HAND_SIZE);
			renderEngine->resourceData(resId.hand)->setColors(bodyColors);
			resId.knee = renderEngineHelper->createCube(KNEE_SIZE);
			renderEngine->resourceData(resId.knee)->setColors(bodyColors);
			resId.foot = renderEngineHelper->createCube(FOOT_SIZE);
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

	void HumanCharacter::mapBodyPointToWorldPoint(int instId, BodyPoint::BodyPart bodyPoint) {
		ResourceInstance *instance;
		if (!m_body)
			return;
		instance = m_cineticoUI.renderEngine()->resourceInstance(instId);
		cinetico_core::Vector3 pos = m_body->bodyPoint(bodyPoint)->position();
		cinetico_core::Vector3 rot = m_body->bodyPoint(bodyPoint)->orientation();
		instance->setPos(render3d::Vector3(pos.x() * CM2W, pos.y() * CM2W, pos.z() * CM2W));
		instance->setRot(render3d::Vector3(rot.x(), rot.y(), rot.z()));
	}

	void HumanCharacter::update() {
		m_body = m_cineticoUI.cinetico().bodyTracker()->body();
		if (m_body) {
			mapBodyPointToWorldPoint(m_instanceIds[0], BodyPoint::Head);
			mapBodyPointToWorldPoint(m_instanceIds[1], BodyPoint::Spine);
			mapBodyPointToWorldPoint(m_instanceIds[2], BodyPoint::LeftElbow);
			mapBodyPointToWorldPoint(m_instanceIds[3], BodyPoint::RightElbow);
			mapBodyPointToWorldPoint(m_instanceIds[4], BodyPoint::LeftPalm);
			mapBodyPointToWorldPoint(m_instanceIds[5], BodyPoint::RightPalm);
			mapBodyPointToWorldPoint(m_instanceIds[6], BodyPoint::LeftKnee);
			mapBodyPointToWorldPoint(m_instanceIds[7], BodyPoint::RightKnee);
			mapBodyPointToWorldPoint(m_instanceIds[8], BodyPoint::LeftFoot);
			mapBodyPointToWorldPoint(m_instanceIds[9], BodyPoint::RightFoot);
		}
	}

	void HumanCharacter::render() {
		if (!m_body || m_body->identifiedBodyPointCount() == 0) return;
		RenderEngine *renderEngine = m_cineticoUI.renderEngine();
		for (unsigned int i = 0; i < m_instanceIds.size(); ++i)
			renderEngine->drawResource(m_instanceIds[i]);
	}
}
