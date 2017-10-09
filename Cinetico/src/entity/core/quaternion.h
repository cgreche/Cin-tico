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

		static Quaternion fromEuler(Vector3 euler) {
			Quaternion q;
			// Abbreviations for the various angular functions
			double cy = cos(euler.y() * 0.5);
			double sy = sin(euler.y() * 0.5);
			double cr = cos(euler.z() * 0.5);
			double sr = sin(euler.z() * 0.5);
			double cp = cos(euler.x() * 0.5);
			double sp = sin(euler.x() * 0.5);

			q.m_w = cy * cr * cp + sy * sr * sp;
			q.m_x = cy * sr * cp - sy * cr * sp;
			q.m_y = cy * cr * sp + sy * sr * cp;
			q.m_z = sy * cr * cp - cy * sr * sp;
			return q;

		}

		Vector3 toEuler() {
			// roll (x-axis rotation)
			float sinr = +2.0 * (m_w * m_x + m_y * m_z);
			float cosr = +1.0 - 2.0 * (m_x * m_x + m_y * m_y);
			float roll = atan2(sinr, cosr);

			// pitch (y-axis rotation)
			float sinp = +2.0 * (m_w * m_y - m_z * m_x);
			float pitch;
			if (fabs(sinp) >= 1)
				pitch = copysign(M_PI / 2, sinp); // use 90 degrees if out of range
			else
				pitch = asin(sinp);

			// yaw (z-axis rotation)
			double siny = +2.0 * (m_w * m_z + m_x * m_y);
			double cosy = +1.0 - 2.0 * (m_y * m_y + m_z * m_z);
			float yaw = atan2(siny, cosy);
			return Vector3(-roll, pitch, yaw);
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
	};

}

#endif