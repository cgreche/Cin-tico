#include "renderengine.h"
#include "viewport.h"

namespace render3d {

	Viewport::Viewport(RenderEngine *engine, int id)
		: RenderObject(engine, id) {
		m_x = 0;
		m_y = 0;
		m_width = 0;
		m_height = 0;
	}

	Viewport::Viewport(RenderEngine *engine, int id, int x, int y, int width, int height)
		: RenderObject(engine, id) {
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;
	}

	void Viewport::setGeometry(int x, int y, int width, int height) {
		m_x = x;
		m_y = y;
		m_width = width;
		m_height = height;
		m_dirtyFlags |= GEOMETRY_DIRTY;
	}

	void Viewport::setPosition(int x, int y) {
		m_x = x;
		m_y = y;
		m_dirtyFlags |= GEOMETRY_DIRTY;
	}

	void Viewport::setSize(int width, int height) {
		m_width = width;
		m_height = height;
		m_dirtyFlags |= GEOMETRY_DIRTY;
	}

	void Viewport::setX(int x) {
		m_x = x;
		m_dirtyFlags |= GEOMETRY_DIRTY;
	}

	void Viewport::setY(int y) {
		m_y = y;
		m_dirtyFlags |= GEOMETRY_DIRTY;
	}

	void Viewport::setWidth(int width) {
		m_width = width;
		m_dirtyFlags |= GEOMETRY_DIRTY;
	}

	void Viewport::setHeight(int height) {
		m_height = height;
		m_dirtyFlags |= GEOMETRY_DIRTY;
	}
}