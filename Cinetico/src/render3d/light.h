#pragma once

#ifndef __RENDER3D_LIGHT_H__
#define __RENDER3D_LIGHT_H__

#include "vector3.h"
#include "color.h"

namespace render3d {

	class Light
	{
		Vector3 m_position;
		Vector3 m_direction;
		Color m_color;

	public:
		Light()
			: m_color(1.f, 1.f, 1.f, 1.f) {
		}

		Light(float red, float green, float blue)
			: m_color(red, green, blue, 1.f) {
		}

	};

}

#endif