#ifndef __CINETICO_CORE_QUATERNION_H__
#define __CINETICO_CORE_QUATERNION_H__

#include "vector3.h"
#include <math.h>

#define M_PI       3.14159265358979323846

namespace cinetico_core {

	class Matrix4x4 {
	public:
		float m[4][4];

		Matrix4x4 transposed() const {
			Matrix4x4 result;
			for (int row = 0; row < 4; ++row) {
				for (int col = 0; col < 4; ++col) {
					result.m[col][row] = m[row][col];
				}
			}
			return result;
		}

		friend inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
	};

	inline Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 ret;
		
		ret.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[1][0] * m2.m[0][1] + m1.m[2][0] * m2.m[0][2] + m1.m[3][0] * m2.m[0][3];
		ret.m[0][1] = m1.m[0][1] * m2.m[0][0] + m1.m[1][1] * m2.m[0][1] + m1.m[2][1] * m2.m[0][2] + m1.m[3][1] * m2.m[0][3];
		ret.m[0][2] = m1.m[0][2] * m2.m[0][0] + m1.m[1][2] * m2.m[0][1] + m1.m[2][2] * m2.m[0][2] + m1.m[3][2] * m2.m[0][3];
		ret.m[0][3] = m1.m[0][3] * m2.m[0][0] + m1.m[1][3] * m2.m[0][1] + m1.m[2][3] * m2.m[0][2] + m1.m[3][3] * m2.m[0][3];
		
		ret.m[1][0] = m1.m[0][0] * m2.m[1][0] + m1.m[1][0] * m2.m[1][1] + m1.m[2][0] * m2.m[1][2] + m1.m[3][0] * m2.m[1][3];
		ret.m[1][1] = m1.m[0][1] * m2.m[1][0] + m1.m[1][1] * m2.m[1][1] + m1.m[2][1] * m2.m[1][2] + m1.m[3][1] * m2.m[1][3];
		ret.m[1][2] = m1.m[0][2] * m2.m[1][0] + m1.m[1][2] * m2.m[1][1] + m1.m[2][2] * m2.m[1][2] + m1.m[3][2] * m2.m[1][3];
		ret.m[1][3] = m1.m[0][3] * m2.m[1][0] + m1.m[1][3] * m2.m[1][1] + m1.m[2][3] * m2.m[1][2] + m1.m[3][3] * m2.m[1][3];

		ret.m[2][0] = m1.m[0][0] * m2.m[2][0] + m1.m[1][0] * m2.m[2][1] + m1.m[2][0] * m2.m[2][2] + m1.m[3][0] * m2.m[2][3];
		ret.m[2][1] = m1.m[0][1] * m2.m[2][0] + m1.m[1][1] * m2.m[2][1] + m1.m[2][1] * m2.m[2][2] + m1.m[3][1] * m2.m[2][3];
		ret.m[2][2] = m1.m[0][2] * m2.m[2][0] + m1.m[1][2] * m2.m[2][1] + m1.m[2][2] * m2.m[2][2] + m1.m[3][2] * m2.m[2][3];
		ret.m[2][3] = m1.m[0][3] * m2.m[2][0] + m1.m[1][3] * m2.m[2][1] + m1.m[2][3] * m2.m[2][2] + m1.m[3][3] * m2.m[2][3];

		ret.m[3][0] = m1.m[0][0] * m2.m[3][0] + m1.m[1][0] * m2.m[3][1] + m1.m[2][0] * m2.m[3][2] + m1.m[3][0] * m2.m[3][3];
		ret.m[3][1] = m1.m[0][1] * m2.m[3][0] + m1.m[1][1] * m2.m[3][1] + m1.m[2][1] * m2.m[3][2] + m1.m[3][1] * m2.m[3][3];
		ret.m[3][2] = m1.m[0][2] * m2.m[3][0] + m1.m[1][2] * m2.m[3][1] + m1.m[2][2] * m2.m[3][2] + m1.m[3][2] * m2.m[3][3];
		ret.m[3][3] = m1.m[0][3] * m2.m[3][0] + m1.m[1][3] * m2.m[3][1] + m1.m[2][3] * m2.m[3][2] + m1.m[3][3] * m2.m[3][3];

		return ret;
	}


	class Quaternion
	{
		float m_x;
		float m_y;
		float m_z;
		float m_w;

	public:
		Quaternion() {
			m_w = 1;
			m_x = 0;
			m_y = 0;
			m_z = 0;
		}

		Quaternion(float w, float x, float y, float z) {
			m_w = w; 
			m_x = x;
			m_y = y;
			m_z = z;
		}

		void normalize() {
			float mag = sqrtf(m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z);
			m_w /= mag;
			m_x /= mag;
			m_y /= mag;
			m_z /= mag;
		}

		Quaternion normalized() {
			float mag = sqrtf(m_w * m_w + m_x * m_x + m_y * m_y + m_z * m_z);
			return Quaternion(m_w / mag, m_x / mag, m_y / mag, m_z / mag);
		}

		Quaternion inverse() const {
			double len = (double)m_w * (double)m_w + (double)m_x * (double)m_x + (double)m_y * (double)m_y + (double)m_z * (double)m_z;
			if(len != 0.f)
				return Quaternion((float)((double)m_w / len), (float)((double)-m_x / len), (float)((double)-m_y / len), (float)((double)-m_z / len));
			return Quaternion(0.0f, 0.0f, 0.0f, 0.0f);
		}

		Quaternion Quaternion::conjugated() const {
			return Quaternion(m_w, -m_x, -m_y, -m_z);
		}

		static float dotProduct(const Quaternion &q1, const Quaternion &q2) {
			return q1.m_w * q2.m_w + q1.m_x * q2.m_x + q1.m_y * q2.m_y + q1.m_z * q2.m_z;
		}

		Vector3 Quaternion::rotatedVector(const Vector3& vector) const {
			Quaternion res = *this * Quaternion(0,vector.x(),vector.y(),vector.z()) * conjugated();
			return Vector3(res.x(),res.y(),res.z());
		}

		static Quaternion nlerp(const Quaternion& q1, const Quaternion& q2, float t) {

			// Handle the easy cases first.
			if (t <= 0.0f) return q1;
			if (t >= 1.0f) return q2;

			// Determine the angle between the two quaternions.
			Quaternion q2b(q2);
			float dot = Quaternion::dotProduct(q1, q2);
			if (dot < 0.0f)
				q2b = -q2b;

			// Perform the linear interpolation.
			Quaternion qret = (q1 * (1.0f - t) + q2b * t);
			return qret.normalized();
		}

		static Quaternion fromEuler(float pitch, float yaw, float roll) {
			float c1 = std::cosf(yaw*0.5f);
			float s1 = std::sinf(yaw*0.5f);
			float c2 = std::cosf(roll*0.5f);
			float s2 = std::sinf(roll*0.5f);
			float c3 = std::cosf(pitch*0.5f);
			float s3 = std::sinf(pitch*0.5f);
			float c1c2 = c1 * c2;
			float s1s2 = s1 * s2;

			const float w = c1c2 * c3 + s1s2 * s3;
			const float x = c1c2 * s3 + s1s2 * c3;
			const float y = s1 * c2 * c3 - c1 * s2 * s3;
			const float z = c1 * s2 * c3 - s1 * c2 * s3;

			return Quaternion(w, x, y, z);
		}

		Vector3 toEuler() const {
			float pitch, yaw, roll;

			// Algorithm from:
			// http://www.j3d.org/matrix_faq/matrfaq_latest.html#Q37
			float xx = m_x * m_x;
			float xy = m_x * m_y;
			float xz = m_x * m_z;
			float xw = m_x * m_w;
			float yy = m_y * m_y;
			float yz = m_y * m_z;
			float yw = m_y * m_w;
			float zz = m_z * m_z;
			float zw = m_z * m_w;

#define E 0.000001f
			const float lengthSquared = xx + yy + zz + m_w * m_w;
			float test = lengthSquared - 1.0f;
			if (!(fabsf(lengthSquared - 1.0f) < E) && !(fabsf(lengthSquared) < E)) {
				xx /= lengthSquared;
				xy /= lengthSquared; // same as (xp / length) * (yp / length)
				xz /= lengthSquared;
				xw /= lengthSquared;
				yy /= lengthSquared;
				yz /= lengthSquared;
				yw /= lengthSquared;
				zz /= lengthSquared;
				zw /= lengthSquared;
			}

			pitch = std::asinf(-2.0f * (yz - xw));

			if (pitch < M_PI/2) {
				if (pitch > -(M_PI/2)) {
					yaw = std::atan2f(2.0f * (xz + yw), 1.0f - 2.0f * (xx + yy));
					roll = std::atan2f(2.0f * (xy + zw), 1.0f - 2.0f * (xx + zz));
				}
				else {
					// not a unique solution
					roll = 0.0f;
					yaw = -std::atan2f(-2.0f * (xy - zw), 1.0f - 2.0f * (yy + zz));
				}
			}
			else {
				// not a unique solution
				roll = 0.0f;
				yaw = std::atan2f(-2.0f * (xy - zw), 1.0f - 2.0f * (yy + zz));
			}

			return Vector3(pitch, yaw, roll);
		}

		Matrix4x4 toRotationMatrix() {
			Matrix4x4 matrix;
			float xy = m_x * m_y;
			float xz = m_x * m_z;
			float xw = m_x * m_w;
			float yz = m_y * m_z;
			float yw = m_y * m_w;
			float zw = m_z * m_w;
			float xSquared = m_x * m_x;
			float ySquared = m_y * m_y;
			float zSquared = m_z * m_z;
			matrix.m[0][0] = 1 - 2 * (ySquared + zSquared);
			matrix.m[0][1] = 2 * (xy - zw);
			matrix.m[0][2] = 2 * (xz + yw);
			matrix.m[0][3] = 0;
			matrix.m[1][0] = 2 * (xy + zw);
			matrix.m[1][1] = 1 - 2 * (xSquared + zSquared);
			matrix.m[1][2] = 2 * (yz - xw);
			matrix.m[1][3] = 0;
			matrix.m[2][0] = 2 * (xz - yw);
			matrix.m[2][1] = 2 * (yz + xw);
			matrix.m[2][2] = 1 - 2 * (xSquared + ySquared);
			matrix.m[2][3] = 0;
			matrix.m[3][0] = 0;
			matrix.m[3][1] = 0;
			matrix.m[3][2] = 0;
			matrix.m[3][3] = 1;
			return matrix;
		}

		static Quaternion fromRotationMatrix(Matrix4x4 matrix) {
			float w, x, y, z;
			float diagonal = matrix.m[0][0] + matrix.m[1][1] + matrix.m[2][2];
			if (diagonal > 0) {
				float w4 = (float)(sqrt(diagonal + 1.f) * 2.f);
				w = w4 / 4.f;
				x = (matrix.m[2][1] - matrix.m[1][2]) / w4;
				y = (matrix.m[0][2] - matrix.m[2][0]) / w4;
				z = (matrix.m[1][0] - matrix.m[0][1]) / w4;
			}
			else if ((matrix.m[0][0] > matrix.m[1][1]) && (matrix.m[0][0] > matrix.m[2][2])) {
				float x4 = (float)(sqrt(1.f + matrix.m[0][0] - matrix.m[1][1] - matrix.m[2][2]) * 2.f);
				w = (matrix.m[2][1] - matrix.m[1][2]) / x4;
				x = x4 / 4.f;
				y = (matrix.m[0][1] + matrix.m[1][0]) / x4;
				z = (matrix.m[0][2] + matrix.m[2][0]) / x4;
			}
			else if (matrix.m[1][1] > matrix.m[2][2]) {
				float y4 = (float)(sqrt(1.f + matrix.m[1][1] - matrix.m[0][0] - matrix.m[2][2]) * 2.f);
				w = (matrix.m[0][2] - matrix.m[2][0]) / y4;
				x = (matrix.m[0][1] + matrix.m[1][0]) / y4;
				y = y4 / 4.f;
				z = (matrix.m[1][2] + matrix.m[2][1]) / y4;
			}
			else {
				float z4 = (float)(sqrt(1.f + matrix.m[2][2] - matrix.m[0][0] - matrix.m[1][1]) * 2.f);
				w = (matrix.m[1][0] - matrix.m[0][1]) / z4;
				x = (matrix.m[0][2] + matrix.m[2][0]) / z4;
				y = (matrix.m[1][2] + matrix.m[2][1]) / z4;
				z = z4 / 4.f;
			}
			return Quaternion(w, x, y, z);
		}

		Quaternion &operator*=(const Quaternion &q) {
			*this = *this * q;
			return *this;
		}

		float w() const { return m_w; }
		float x() const { return m_x; }
		float y() const { return m_y; }
		float z() const { return m_z; }

		friend inline bool operator==(const Quaternion &q1, const Quaternion &q2);
		friend inline bool operator!=(const Quaternion &q1, const Quaternion &q2);
		friend inline const Quaternion operator+(const Quaternion &q1, const Quaternion &q2);
		friend inline const Quaternion operator-(const Quaternion &q1, const Quaternion &q2);
		friend inline const Quaternion operator*(const Quaternion &q1, const Quaternion &q2);
		friend inline const Quaternion operator*(const Quaternion &q, float factor);
		friend inline const Quaternion operator-(const Quaternion &q);
	};

	inline bool operator==(const Quaternion &q1, const Quaternion &q2) {
		return q1.m_w == q2.m_w && q1.m_x == q2.m_x && q1.m_y == q2.m_y && q1.m_z == q2.m_z;
	}

	inline bool operator!=(const Quaternion &q1, const Quaternion &q2) {
		return q1.m_w != q2.m_w || q1.m_x != q2.m_x || q1.m_y != q2.m_y || q1.m_z != q2.m_z;
	}

	inline const Quaternion operator+(const Quaternion &q1, const Quaternion &q2) {
		return Quaternion(q1.m_w + q2.m_w, q1.m_x + q2.m_x, q1.m_y + q2.m_y, q1.m_z + q2.m_z);
	}

	inline const Quaternion operator-(const Quaternion &q1, const Quaternion &q2) {
		return Quaternion(q1.m_w - q2.m_w, q1.m_x - q2.m_x, q1.m_y - q2.m_y, q1.m_z - q2.m_z);
	}

	inline const Quaternion operator*(const Quaternion &q, float factor) {
		return Quaternion(q.m_w * factor, q.m_x * factor, q.m_y * factor, q.m_z * factor);
	}

	inline const Quaternion operator*(const Quaternion &q1, const Quaternion& q2) {
		float _w = (q1.m_z + q1.m_x) * (q2.m_x + q2.m_y);
		float _y = (q1.m_w - q1.m_y) * (q2.m_w + q2.m_z);
		float _z = (q1.m_w + q1.m_y) * (q2.m_w - q2.m_z);
		float _x = _w + _y + _z;
		float _q = 0.5f * (_x + (q1.m_z - q1.m_x) * (q2.m_x - q2.m_y));
		float w = _q - _w + (q1.m_z - q1.m_y) * (q2.m_y - q2.m_z);
		float x = _q - _x + (q1.m_x + q1.m_w) * (q2.m_x + q2.m_w);
		float y = _q - _y + (q1.m_w - q1.m_x) * (q2.m_y + q2.m_z);
		float z = _q - _z + (q1.m_z + q1.m_y) * (q2.m_w - q2.m_x);

		return Quaternion(w, x, y, z);
	}

	inline const Quaternion operator-(const Quaternion &q) {
		return Quaternion(-q.m_w, -q.m_x, -q.m_y, -q.m_z);
	}
}

#endif