#ifndef __CINETICO_UI_DUMMYCHARACTER_H__
#define __CINETICO_UI_DUMMYCHARACTER_H__

#include "character.h"

namespace cinetico {

	class CineticoUI;

	class DummyCharacter : public Character {

	public:
		DummyCharacter(CineticoUI &cineticoUI);

		virtual void update();
		virtual void render();
	};

}

#endif