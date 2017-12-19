#ifndef __CINETICO_UI_HADOUKEN_H__
#define __CINETICO_UI_HADOUKEN_H__

#include "character.h"
#include "core/entity/Body.h"
#include "core/entity/BodyPoint.h"

namespace cinetico {

	using namespace cinetico_core;

	class Hadouken {
	protected:
		CineticoUI &m_cineticoUI;

		int m_materialId;
		int m_instId;
		cinetico_core::Vector3 m_position;
		cinetico_core::Vector3 m_velocity;

		float m_ttl;
		uilib::u64 m_creationTime;
		uilib::u64 m_curTime;
		bool m_alive;

	public:
		Hadouken(CineticoUI &cineticoUI, int resId, float scale);

		virtual void setPosition(const cinetico_core::Vector3 &position) { m_position = position; }
		virtual void setVelocity(const cinetico_core::Vector3 &velocity) { m_velocity = velocity; }

		virtual void update();
		virtual void render();

		bool alive() const { return m_alive; }
	};

}

#endif