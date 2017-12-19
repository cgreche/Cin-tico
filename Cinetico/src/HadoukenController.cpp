
#include "cineticoui.h"
#include "HadoukenController.h"
#include "humancharacter.h"
#include "hadouken.h"
#include <uilib/lib/time.h>

namespace cinetico {

	HadoukenController::HadoukenController(CineticoUI &cineticoUI, HumanCharacter &humanCharacter)
		: m_cineticoUI(cineticoUI)
		, m_humanCharacter(humanCharacter) {
		std::vector<int> resIds = cineticoUI.renderEngineHelper()->loadModel("resources/ball.dae");
		ResourceData* data = cineticoUI.renderEngine()->resourceData(resIds[0]);
		int vertexCount = data->vertexCount();
		
		//render3d::Color *colors = new render3d::Color[vertexCount];
		//for (int i = 0; i < vertexCount; ++i)
		//	colors[i] = render3d::Color(0,0,255);
		//data->setColors(colors);
		//delete[] colors;
		
		m_hadouResId = resIds[0];
		m_cond = 0;
	}

	void HadoukenController::createHadouken(const cinetico_core::Vector3 &hadouPos, const cinetico_core::Vector3 &hadouVel, float scale) {
		Hadouken *hadouken = new Hadouken(m_cineticoUI, m_hadouResId,scale);
		hadouken->setPosition(hadouPos);
		hadouken->setVelocity(hadouVel);
		m_hadouList.push_back(hadouken);
	}

	cinetico_core::Vector3 getDir(Body *body, BodyPoint::BodyPart bp, cinetico_core::Vector3 transformVec = cinetico_core::Vector3(0, 1, 0)) {
		return body->bodyPoint(bp)->orientation().rotatedVector(transformVec);
	}

	void HadoukenController::step() {
		Body *body = m_humanCharacter.body();

		for (int i = 0; i < m_hadouList.size(); ++i) {
			m_hadouList[i]->update();
			if (!m_hadouList[i]->alive()) {
				delete m_hadouList[i];
				m_hadouList.erase(m_hadouList.begin() + i);
				break;
			}
		}

		if (!body || body->identifiedBodyPointCount() == 0) {
			m_cond = 0;
			return;
		}

		cinetico_core::Vector3 lElbowRot = getDir(body, BodyPoint::LeftElbow);
		cinetico_core::Vector3 rElbowRot = getDir(body, BodyPoint::RightElbow);
		cinetico_core::Vector3 pelvisRot = getDir(body, BodyPoint::Pelvis);
		cinetico_core::Vector3 pelvisBinormal = getDir(body, BodyPoint::Pelvis, cinetico_core::Vector3(0, 0, -1));
		float dot = dotProduct(lElbowRot, rElbowRot);
		float dot2 = dotProduct(lElbowRot, pelvisBinormal);
		float dot3 = dotProduct(rElbowRot, pelvisBinormal);

		static render3d::Vector3 hadoPos;
		static render3d::Vector3 hadoDir;

		uilib::u64 curTime = m_cineticoUI.cinetico().currentTime();
		uilib::u64 holdTime;

		if (dot >= -0.3f && dot <= 0.3f && dot2 >= 0.8f) {
			if (m_cond == 0) {
				++m_cond;
				m_initTime = m_cineticoUI.cinetico().currentTime();
				m_holdTime = 0;
			}
			else if (m_cond == 1) {
				m_holdTime = curTime - m_initTime;
				if (m_holdTime / uilib::OSTime::ticksPerSecond() >= 5)
					int a = 1;
			}
		}

		if (dot >= 0.9f) {
			if (m_cond == 1) {
				m_cond = 2;
				if (m_holdTime / uilib::OSTime::ticksPerSecond() >= 5) {
					m_cond = 3;
				}
			}
		}
		else {
			if (m_cond == 3)
				m_cond = 0;
		}

		if (dot <= -0.8f) {
			m_cond = 0;
		}

		if (m_cond == 2 || m_cond == 3) {
			float hadoSpeed = 0.3f;
			cinetico_core::Vector3 newp = (body->bodyPoint(BodyPoint::LeftElbow)->position() + body->bodyPoint(BodyPoint::RightElbow)->position()) / 2;
			cinetico_core::Vector3 newd = (lElbowRot + rElbowRot / 2)*hadoSpeed;
			createHadouken(newp, newd, m_cond == 2 ? 0.5f : 1.0f);
			if (m_cond == 2)
				m_cond = 0;
		}
	}

	void HadoukenController::render() {
		for (Hadouken *hadou : m_hadouList)
			hadou->render();
	}

}
