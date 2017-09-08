#pragma once

#ifndef __RENDER3D_VERTEX3_H__
#define __RENDER3D_VERTEX3_H__

namespace render3d {

	class Vertex3
	{
		float m_x, m_y, m_z;

	public:
		Vertex3() {
			m_x = m_y = m_z = 0.0f;
		}

		Vertex3(float x, float y, float z) {
			m_x = x;
			m_y = y;
			m_z = z;
		}
	};

}

#endif