
#include "cineticoui.h"
#include "HadoukenController.h"
#include "humancharacter.h"
#include "hadouken.h"

namespace cinetico {

	HadoukenController::HadoukenController(CineticoUI &cineticoUI, HumanCharacter &humanCharacter)
		: m_cineticoUI(cineticoUI)
		, m_humanCharacter(humanCharacter) {
		std::vector<int> resIds = cineticoUI.renderEngineHelper()->loadModel("resources/ball.dae");
		m_hadouResId = resIds[0];
		m_cond = 0;
	}

	void HadoukenController::createHadouken(const cinetico_core::Vector3 &hadouPos, const cinetico_core::Vector3 &hadouVel) {
		Hadouken *hadouken = new Hadouken(m_cineticoUI, m_hadouResId);
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
		static int cond = 0;
		static int cond2 = 0;
		static render3d::Vector3 hadoPos;
		static render3d::Vector3 hadoDir;

		if (m_cond == 0 && dot >= -0.3f && dot <= 0.3f && dot2 >= 0.8f)
			++m_cond;
		if (cond == 1 && dot >= 0.9f)
			++m_cond;
		if (dot <= -0.8f) {
			m_cond = 0;
		}
		if (cond == 2) {
			if (cond2 == 0) {
				float hadoSpeed = 0.3f;
				cinetico_core::Vector3 newp = (body->bodyPoint(BodyPoint::LeftElbow)->position() + body->bodyPoint(BodyPoint::RightElbow)->position()) / 2;
				cinetico_core::Vector3 newd = (lElbowRot + rElbowRot / 2)*hadoSpeed;
				createHadouken(newp, newd);
				++cond2;
			}
			else if (cond2 == 1) {
			}
		}
	}

	void HadoukenController::render() {
		for (Hadouken *hadou : m_hadouList)
			hadou->render();
	}

}
