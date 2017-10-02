#pragma once

#ifndef __RENDER3D_LIGHT_H__
#define __RENDER3D_LIGHT_H__

namespace render3d {

	class Light
	{
		float m_red, m_green, m_blue;

	public:
		Light() {
			m_red = m_green = m_blue = 1.0f;
		}

		Light(float red, float green, float blue) {
			m_red = red;
			m_green = green;
			m_blue = blue;
		}

	};

}

#endif