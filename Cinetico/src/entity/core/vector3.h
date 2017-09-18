#ifndef __CINETICO_CORE_VECTOR3_H__
#define __CINETICO_CORE_VECTOR3_H__

namespace cinetico_core {

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

		inline void set(float x, float y, float z) {
			m_x = x;
			m_y = y;
			m_z = z;
		}

		inline void setX(float x) { m_x = x; }
		inline void setY(float y) { m_y = y; }
		inline void setZ(float z) { m_z = z; }

		inline float x() const { return m_x; }
		inline float y() const { return m_y; }
		inline float z() const { return m_z; }
	};

}

#endif