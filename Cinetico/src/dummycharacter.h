#ifndef __CINETICO_3D_DUMMYCHARACTER_H__
#define __CINETICO_3D_DUMMYCHARACTER_H__

#include "character.h"

namespace cinetico {

	class Cinetico3D;

	class DummyCharacter : public Character {

	public:
		DummyCharacter(Cinetico3D &cinetico3d);

		virtual void update();
		virtual void render();
	};

}

#endif