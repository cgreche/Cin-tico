#pragma once

#ifndef __RENDER3D_VECTOR3_H__
#define __RENDER3D_VECTOR3_H__

namespace render3d {

	class Vector3
	{
		float m_x, m_y, m_z;

	public:
		Vector3() {
			m_x = m_y = m_z = 0.0f;
		}

		Vector3(float x, float y, float z) {
			m_x = x;
			m_y = y;
			m_z = z;
		}

		void set(float x, float y, float z) {
			m_x = x;
			m_y = y;
			m_z = z;
		}

		void setX(float x) { m_x = x; }
		void setY(float y) { m_y = y; }
		void setZ(float z) { m_z = z; }

		float x() const { return m_x; }
		float y() const { return m_y; }
		float z() const { return m_z; }
	};

}

#endif