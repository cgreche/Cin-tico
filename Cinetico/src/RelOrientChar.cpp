
#include "cineticoui.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"
#include "RelOrientChar.h"

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

	static render3d::Color boneColors[] =
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

	static render3d::Vector3 bonePositions[100];
	static int vertexCount = 0;

	static render3d::Color bodyColors[] =
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

	int g_resBoneId;
	int g_instBoneId[BodyPoint::BodyPartCount];

	static bool RoBodyModelLoaded = false;

	RelOrientChar::RelOrientChar(CineticoUI &cineticoUI)
		: Character(cineticoUI) {

			RenderEngine *renderEngine = m_cineticoUI.renderEngine();
			RenderEngineHelper *renderEngineHelper = m_cineticoUI.renderEngineHelper();

			g_resBoneId = renderEngineHelper->createRectangularPrism(0.2f,0.45f,0.2f);
			renderEngine->resourceData(g_resBoneId)->setColors(bodyColors);

			for (int i = 0; i < BodyPoint::BodyPartCount; ++i)
				g_instBoneId[i] = renderEngine->newResourceInstance(g_resBoneId);

	}

	inline void RelOrientChar::mapBodyPointToCharacterBodyPointNode(BodyPoint::BodyPart bodyPoint) {
		cinetico_core::Quaternion rot = m_body->bodyPoint(bodyPoint)->orientation();
		m_bodyPoints[bodyPoint]->setGlobalOrientation(rot);
	}

	void RelOrientChar::update() {
		m_body = m_cineticoUI.cinetico().bodyTracker()->body();
		if (m_body) {
			//cabeça até cervical
			int index = 0;
			for (int i = 0; i < BodyPoint::BodyPartCount; ++i) {
				mapBodyPointToCharacterBodyPointNode((BodyPoint::BodyPart)i);
			}

			vertexCount = index;
		}
		//Character::update();
	}

	static int level = 0;
	void RelOrientChar::renderBone(BodyPointNode* node) {
		if (!node || node->childCount() == 0)
			return;
		if (level > 3)
			return;
		ResourceInstance *inst = m_cineticoUI.renderEngine()->resourceInstance(g_instBoneId[node->bp()]);
		BodyPointNode* parentNode = node->parent();
		if (parentNode) {
			ResourceInstance *instParent = m_cineticoUI.renderEngine()->resourceInstance(g_instBoneId[parentNode->bp()]);
			cinetico_core::Vector3 dir = parentNode->globalOrientation().rotatedVector(cinetico_core::Vector3(0, 1, 0));

			cinetico_core::Vector3 boneSize = cinetico_core::Vector3(0.2f, 0.45f, 0.2f);
			cinetico_core::Vector3 halfPos = (cinetico_core::Vector3(instParent->pos().x(), instParent->pos().y(), instParent->pos().z()) + boneSize*dir);
			inst->setPos(render3d::Vector3(halfPos.x(),halfPos.y(),halfPos.z()));
		}
		inst->setRotation(node->globalOrientation());
		m_cineticoUI.renderEngine()->drawResource(g_instBoneId[node->bp()]);
		for (int i = 0; i < node->childCount(); ++i) {
			BodyPointNode *child = node->child(i);
			++level;
			renderBone(child);
		}
	}

	void RelOrientChar::render() {
		if (!m_body || m_body->identifiedBodyPointCount() == 0) return;
		level = 0;
		m_cineticoUI.renderEngine()->resourceInstance(g_instBoneId[m_rootBodyPoint->bp()])->setPos(render3d::Vector3(3.0f,m_position.y()+0.8,m_position.z()));
		renderBone(m_rootBodyPoint);

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
