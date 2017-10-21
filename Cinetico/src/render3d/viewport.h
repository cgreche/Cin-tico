
#ifndef __RENDER3D_VIEWPORT_H__
#define __RENDER3D_VIEWPORT_H__

#include "renderobject.h"

namespace render3d {

	class RenderEngine;

	class Viewport : public RenderObject
	{
	public:
		enum DirtyFlags {
			CLEAR_DIRTY = 1,
			GEOMETRY_DIRTY = 2
		};

	private:
		int m_x;
		int m_y;
		int m_width;
		int m_height;

		Viewport(RenderEngine *engine, int id);
		Viewport(RenderEngine *engine, int id, int x, int y, int width, int height);

	public:
		friend class RenderEngine;

		void setGeometry(int x, int y, int width, int height);
		void setPosition(int x, int y);
		void setSize(int width, int height);
		void setX(int x);
		void setY(int y);
		void setWidth(int width);
		void setHeight(int height);

		int x() const { return m_x; }
		int y() const { return m_y; }
		int width() const { return m_width; }
		int height() const { return m_height; }
	};

}

#endif