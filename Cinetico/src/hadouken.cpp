
#include "cineticoui.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"
#include "humancharacter.h"
#include "hadouken.h"
#include "uilib/lib/time.h"

namespace cinetico {

	using namespace render3d;
	using render3d::Color;

#define CM2W 2

	Hadouken::Hadouken(CineticoUI &cineticoUI, int resId, float scale) 
		: m_cineticoUI(cineticoUI) {

		RenderEngine *renderEngine = m_cineticoUI.renderEngine();
		RenderEngineHelper *renderEngineHelper = m_cineticoUI.renderEngineHelper();

		m_materialId = renderEngine->newMaterial(Color(0,0,255));
		m_instId = renderEngine->newResourceInstance(resId);
		renderEngine->resourceInstance(m_instId)->setMaterial(m_materialId);
		renderEngine->resourceInstance(m_instId)->setScale(scale);

		m_ttl = 3.0f;
		m_curTime = m_cineticoUI.cinetico().currentTime();
		m_alive = true;
	}

	void Hadouken::update() {

		uilib::u64 time = m_cineticoUI.cinetico().currentTime();
		uilib::u64 diff = (time - m_curTime) / uilib::OSTime::ticksPerSecond();
		if (diff >= 3.0f) {
			//todo: set alive to false
		}
		else {
			m_position += m_velocity;
			m_curTime = time;
		}
	}

	void Hadouken::render() {
		ResourceInstance *inst = m_cineticoUI.renderEngine()->resourceInstance(m_instId);
		render3d::Vector3 rendPos = render3d::Vector3(m_position.x(), m_position.y(), m_position.z());
		rendPos *= CM2W;
		inst->setPos(rendPos);
		m_cineticoUI.renderEngine()->drawResource(m_instId);
	}

}
