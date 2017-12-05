#ifndef __CINETICO_UI_HUMANCHARACTER_H__
#define __CINETICO_UI_HUMANCHARACTER_H__

#include "character.h"
#include "core/entity/Body.h"
#include "core/entity/BodyPoint.h"

namespace cinetico {

	using namespace cinetico_core;

	class HumanCharacter : public Character {
		Body *m_body;

		void mapBodyPointNodeToWorldPoint(int instId, BodyPoint::BodyPart bodyPoint, bool mirrored = false);
		inline void mapBodyPointToCharacterBodyPointNode(BodyPoint::BodyPart bodyPoint);
	public:
		HumanCharacter(CineticoUI &cinetico);

		virtual void update();
		virtual void render();

		Body *body() const { return m_body; }
	};

}

#endif