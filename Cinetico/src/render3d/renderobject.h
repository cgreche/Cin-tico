#pragma once

#ifndef __RENDER3D_RENDEROBJECT_H__
#define __RENDER3D_RENDEROBJECT_H__

namespace render3d {

	class RenderEngine;

	class RenderObject
	{
		friend class RenderEngine;

	protected:
		RenderEngine *m_engine;
		int m_id;
		void *m_internalData;
		bool m_active;
		unsigned long m_dirtyFlags;

		RenderObject(RenderEngine *engine, int id) {
			m_engine = engine;
			m_id = id;
			m_internalData = 0;
			m_active = false;
			m_dirtyFlags = 0;
		}

	public:
		void setActive(bool active) { m_active = active; }
		void setDirtyFlags(unsigned long flags) { m_dirtyFlags = flags; }

		int id() const { return m_id; }
		void *internalData() const { return m_internalData; }
		bool active() const { return m_active; }
		unsigned long dirtyFlags() const { return m_dirtyFlags; }
	};

}

#endif