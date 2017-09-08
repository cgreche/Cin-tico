
#ifndef __RENDER3D_VIEWPORT_H__
#define __RENDER3D_VIEWPORT_H__

#include "renderobject.h"

namespace render3d {

	class Viewport : public RenderObject
	{
	public:
		enum DirtyFlags {
			CLEAR_DIRTY = 1
		};

	private:
		int m_x;
		int m_y;
		int m_width;
		int m_height;

	public:
		friend class RenderEngine;

		Viewport(RenderEngine *engine, int id)
			: RenderObject(engine, id) {
			m_x = 0;
			m_y = 0;
			m_width = 0;
			m_height = 0;
		}

		Viewport(RenderEngine *engine, int id, int x, int y, int width, int height)
			: RenderObject(engine, id) {
			m_x = x;
			m_y = y;
			m_width = width;
			m_height = height;
		}

		int x() const { return m_x; }
		int y() const { return m_y; }
		int width() const { return m_width; }
		int height() const { return m_height; }
	};

}

#endif