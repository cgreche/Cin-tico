#ifndef __CINETICO_3D_CHARACTER_H__
#define __CINETICO_3D_CHARACTER_H__

#include <vector>
#include "cinetico3d.h"
#include "entity/core/vector3.h"

namespace cinetico {

	class Cinetico3D;

	class Character {
	protected:
		Cinetico3D &m_cinetico3d;

		std::vector<int> m_instanceIds;
		cinetico_core::Vector3 m_position;

	public:
		Character(Cinetico3D &cinetico3d);

		virtual void setPosition(const cinetico_core::Vector3 &position) { m_position = position; }

		virtual void update() = 0;
		virtual void render() = 0;
	};
	
}

#endif