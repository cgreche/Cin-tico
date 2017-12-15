#ifndef __CINETICO_UI_HADOUKEN_CONTROLLER_H__
#define __CINETICO_UI_HADOUKEN_CONTROLLER_H__

#include <vector>
#include <core/lib/vector3.h>

namespace cinetico {

	using namespace cinetico_core;
	class CineticoUI;
	class HumanCharacter;
	class Hadouken;

	class HadoukenController {

		CineticoUI &m_cineticoUI;

		int m_hadouResId;
		HumanCharacter &m_humanCharacter;
		std::vector<Hadouken*> m_hadouList;
		int m_cond;
		uilib::u64 m_initTime;
		uilib::u64 m_holdTime;

	public:
		HadoukenController(CineticoUI &cineticoUI, HumanCharacter &humanCharacter);
		void step();
		void render();
		void createHadouken(const cinetico_core::Vector3 &hadouPos, const cinetico_core::Vector3 &hadouVel, float scale);
	};

}

#endif