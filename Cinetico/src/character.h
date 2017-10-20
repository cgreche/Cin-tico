#ifndef __CINETICO_UI_CHARACTER_H__
#define __CINETICO_UI_CHARACTER_H__

#include <vector>
#include "entity/core/vector3.h"

namespace cinetico {

	class CineticoUI;

	class Character {
	protected:
		CineticoUI &m_cineticoUI;

		std::vector<int> m_instanceIds;
		cinetico_core::Vector3 m_position;

	public:
		Character(CineticoUI &cineticoUI);

		virtual void setPosition(const cinetico_core::Vector3 &position) { m_position = position; }

		virtual void update() = 0;
		virtual void render() = 0;
	};
	
}

#endif