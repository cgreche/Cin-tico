#pragma once

#ifndef __RENDER3D_COLOR_H__
#define __RENDER3D_COLOR_H__

namespace render3d {

	class Color
	{
		int m_r, m_g, m_b, m_a;

	public:
		Color() {
			m_r = m_g = m_b = 0;
			m_a = 255;
		}

		Color(int r, int g, int b) {
			m_r = r;
			m_g = g;
			m_b = b;
			m_a = 255;
		}

		Color(int r, int g, int b, int a) {
			m_r = r;
			m_g = g;
			m_b = b;
			m_a = a;
		}

		Color(float r, float g, float b) {
			m_r = (int)(r * 255.f);
			m_g = (int)(g * 255.f);
			m_b = (int)(b * 255.f);
			m_a = 255;
		}
		Color(float r, float g, float b, float a) {
			m_r = (int)(r * 255.f);
			m_g = (int)(g * 255.f);
			m_b = (int)(b * 255.f);
			m_a = (int)(a * 255.f);
		}

		int r() const { return m_r; }
		int g() const { return m_g; }
		int b() const { return m_b; }
		int a() const { return m_a; }
	};

}

#endif