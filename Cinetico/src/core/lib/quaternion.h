#ifndef __CINETICO_CORE_QUATERNION_H__
#define __CINETICO_CORE_QUATERNION_H__

#include "vector3.h"
#include <math.h>

#define M_PI       3.14159265358979323846

namespace cinetico_core {

	class Matrix4 {
	public:
		float m[4][4];
	};


	class Quaternion
	{
		float m_x;
		float m_y;
		float m_z;
		float m_w;

	public:
		Quaternion() {
			m_x = 0;
			m_y = 0;
			m_z = 0;
			m_w = 1;
		}

		Quaternion(float x, float y, float z, float w) {
			m_x = x;
			m_y = y;
			m_z = z;
			m_w = w;
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
			return Quaternion(m_x / mag, m_y / mag, m_z / mag, m_w / mag);
		}

		static float dotProduct(const Quaternion &q1, const Quaternion &q2) {
			return q1.m_w * q2.m_w + q1.m_x * q2.m_x + q1.m_y * q2.m_y + q1.m_z * q2.m_z;
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
			Quaternion q;
			// Abbreviations for the various angular functions
			float cy = cosf(yaw * 0.5f);
			float sy = sinf(yaw * 0.5f);
			float cr = cosf(roll * 0.5f);
			float sr = sinf(roll * 0.5f);
			float cp = cosf(pitch * 0.5f);
			float sp = sinf(pitch * 0.5f);

			q.m_w = cy * cr * cp + sy * sr * sp;
			q.m_x = cy * cr * sp + sy * sr * cp;
			q.m_y = sy * cr * cp - cy * sr * sp;
			q.m_z = cy * sr * cp - sy * cr * sp;
			return q;
		}

		Vector3 toEuler() const {
			float pitch, yaw, roll;

			float xx = m_x * m_x;
			float xy = m_x * m_y;
			float xz = m_x * m_z;
			float xw = m_x * m_w;
			float yy = m_y * m_y;
			float yz = m_y * m_z;
			float yw = m_y * m_w;
			float zz = m_z * m_z;
			float zw = m_z * m_w;

			const float lengthSquared = xx + yy + zz + m_w * m_w;
			if (lengthSquared != 0.f) {
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


			pitch = std::asin(-2.0f * (yz - xw));
			if (pitch < M_PI/2) {
				if (pitch > -M_PI/2) {
					yaw = std::atan2(2.0f * (xz + yw), 1.0f - 2.0f * (xx + yy));
					roll = std::atan2(2.0f * (xy + zw), 1.0f - 2.0f * (xx + zz));
				}
				else {
					// not a unique solution
					roll = 0.0f;
					yaw = -std::atan2(-2.0f * (xy - zw), 1.0f - 2.0f * (yy + zz));
				}
			}
			else {
				// not a unique solution
				roll = 0.0f;
				yaw = std::atan2(-2.0f * (xy - zw), 1.0f - 2.0f * (yy + zz));
			}

			return Vector3(pitch, yaw, roll);
		}

		Matrix4 toRotationMatrix() {
			Matrix4 matrix;
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

		static Quaternion fromRotationMatrix(Matrix4 matrix) {
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
			return Quaternion(x, y, z, w);
		}

		Quaternion &operator*=(const Quaternion &q) {
			*this = *this * q;
			return *this;
		}

		friend inline const Quaternion operator+(const Quaternion &q1, const Quaternion &q2);
		friend inline const Quaternion operator-(const Quaternion &q1, const Quaternion &q2);
		friend inline const Quaternion operator*(const Quaternion &q1, const Quaternion &q2);
		friend inline const Quaternion operator*(const Quaternion &q, float factor);
		friend inline const Quaternion operator-(const Quaternion &q);
	};


	inline const Quaternion operator+(const Quaternion &q1, const Quaternion &q2) {
		return Quaternion(q1.m_x + q2.m_x, q1.m_y + q2.m_y, q1.m_z + q2.m_z, q1.m_w + q2.m_w);
	}

	inline const Quaternion operator-(const Quaternion &q1, const Quaternion &q2) {
		return Quaternion(q1.m_x - q2.m_x, q1.m_y - q2.m_y, q1.m_z - q2.m_z, q1.m_w - q2.m_w);
	}

	inline const Quaternion operator*(const Quaternion &q, float factor) {
		return Quaternion(q.m_x * factor, q.m_y * factor, q.m_z * factor, q.m_w * factor);
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
		return Quaternion(-q.m_x, -q.m_y, -q.m_z, -q.m_w);
	}
}

#endif