
#ifndef __RENDER3D_TEXTRESOURCE_H__
#define __RENDER3D_TEXTRESOURCE_H__

#include "color.h"
#include "renderobject.h"

namespace render3d {

	class TextResource : public RenderObject
	{
	public:
		enum DirtyFlags {
			COLOR_DIRTY = 1,
			RECT_DIRTY = 2
		};

	private:
		Color m_textColor;
		int m_x;
		int m_y;
		int m_width;
		int m_height;

	public:
		friend class RenderEngine;

		TextResource(RenderEngine *engine, int id)
			: RenderObject(engine, id) {
			m_x = 0;
			m_y = 0;
			m_width = 0;
			m_height = 0;
		}

		TextResource(RenderEngine *engine, int id, int x, int y, int width, int height)
			: RenderObject(engine, id) {
			m_x = x;
			m_y = y;
			m_width = width;
			m_height = height;
		}

		void setPosition(int x, int y) {
			m_x = x;
			m_y = y;
			m_dirtyFlags |= RECT_DIRTY;
		}

		void setSize(int width, int height) {
			m_width = width;
			m_height = height;
			m_dirtyFlags |= RECT_DIRTY;
		}

		int x() const { return m_x; }
		int y() const { return m_y; }
		int width() const { return m_width; }
		int height() const { return m_height; }
	};

}

#endif