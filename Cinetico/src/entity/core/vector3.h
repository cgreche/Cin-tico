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

		inline float lengthSquared() const {
			return m_x*m_x + m_y*m_y + m_z*m_z;
		}

		inline float length() const {
			return std::sqrt(lengthSquared());
		}

		inline Vector3 squared() const {
			return Vector3(m_x*m_x, m_y*m_y, m_z*m_z);
		}

		void normalize() {
			float len = lengthSquared();
			if (len == 0.f)
				return;
			float sqrtLen = std::sqrt(len);
			m_x /= sqrtLen;
			m_y /= sqrtLen;
			m_z /= sqrtLen;
		}

		Vector3 Vector3::normalized() const
		{
			float len = lengthSquared();
			if (len < 1.0f) {
				return *this;
			}
			else if (len > 0.f) {
				float sqrtLen = std::sqrt(len);
				return Vector3(m_x / sqrtLen, m_y / sqrtLen, m_z / sqrtLen);
			}
			
			return Vector3();
		}

		float euclideanDistanceTo(const Vector3 &v) const {
			return (*this - v).length();
		}

		float euclideanDistanceToLine(const Vector3 &v, const Vector3 &direction) const {
			Vector3 p = v + dotProduct(*this - v, direction) * direction;
			return (*this - p).length();
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

		inline Vector3& operator+=(const Vector3 &v) {
			m_x += v.m_x;
			m_y += v.m_y;
			m_z += v.m_z;
			return *this;
		}

		inline Vector3& operator-=(const Vector3 &v) {
			m_x -= v.m_x;
			m_y -= v.m_y;
			m_z -= v.m_z;
			return *this;
		}

		inline Vector3& operator*=(const Vector3 &v) {
			m_x *= v.m_x;
			m_y *= v.m_y;
			m_z *= v.m_z;
			return *this;
		}

		inline Vector3& operator*=(float factor) {
			m_x *= factor;
			m_y *= factor;
			m_z *= factor;
			return *this;
		}

		inline Vector3& Vector3::operator/=(float divisor) {
			m_x /= divisor;
			m_y /= divisor;
			m_z /= divisor;
			return *this;
		}

		inline Vector3 &Vector3::operator/=(const Vector3 &v) {
			m_x /= v.m_x;
			m_y /= v.m_y;
			m_z /= v.m_z;
			return *this;
		}

		friend inline bool operator==(const Vector3 &v1, const Vector3 &v2);
		friend inline bool operator!=(const Vector3 &v1, const Vector3 &v2);
		friend inline const Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
		friend inline const Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
		friend inline const Vector3 operator-(const Vector3 &v);
		friend inline const Vector3 operator*(const Vector3 &v1, const Vector3 &v2);
		friend inline const Vector3 operator*(const Vector3 &v, float factor);
		friend inline const Vector3 operator*(float factor, const Vector3 &v);
		friend inline const Vector3 operator/(const Vector3 &v1, const Vector3 &v2);
		friend inline const Vector3 operator/(const Vector3 &v, float divisor);
		friend inline float dotProduct(const Vector3 &v1, const Vector3 &v2);
		friend inline const Vector3 crossProduct(const Vector3 &v1, const Vector3 &v2);
	};

	inline bool operator==(const Vector3 &v1, const Vector3 &v2) {
		return v1.m_x == v2.m_x && v1.m_y == v2.m_y && v1.m_z == v2.m_z;
	}

	inline bool operator!=(const Vector3 &v1, const Vector3 &v2) {
		return v1.m_x != v2.m_x || v1.m_y != v2.m_y || v1.m_z != v2.m_z;
	}

	inline const Vector3 operator+(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(v1.m_x + v2.m_x, v1.m_y + v2.m_y, v1.m_z + v2.m_z);
	}

	inline const Vector3 operator-(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(v1.m_x - v2.m_x, v1.m_y - v2.m_y, v1.m_z - v2.m_z);
	}

	inline const Vector3 operator-(const Vector3 &v) {
		return Vector3(-v.m_x, -v.m_y, -v.m_z);
	}

	inline const Vector3 operator*(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(v1.m_x * v2.m_x, v1.m_y * v2.m_y, v1.m_z * v2.m_z);
	}

	inline const Vector3 operator*(const Vector3 &v, float factor) {
		return Vector3(v.m_x * factor, v.m_y *factor, v.m_z *factor);
	}

	inline const Vector3 operator*(float factor, const Vector3 &v) {
		return Vector3(v.m_x * factor, v.m_y *factor, v.m_z *factor);
	}

	inline const Vector3 operator/(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(v1.m_x / v2.m_x, v1.m_y / v2.m_y, v1.m_z / v2.m_z);
	}

	inline const Vector3 operator/(const Vector3 &v, float divisor) {
		return Vector3(v.m_x / divisor, v.m_y / divisor, v.m_z / divisor);
	}

	inline float dotProduct(const Vector3& v1, const Vector3& v2) {
		return v1.m_x * v2.m_x + v1.m_y * v2.m_y + v1.m_z * v2.m_z;
	}

	inline const Vector3 crossProduct(const Vector3 &v1, const Vector3 &v2) {
		return Vector3(v1.m_y * v2.m_z - v1.m_z * v2.m_y,
			v1.m_z * v2.m_x - v1.m_x * v2.m_z,
			v1.m_x * v2.m_y - v1.m_y * v2.m_x);
	}

}

#endif