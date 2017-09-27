
#ifndef __RENDER3D_FONTRESOURCE_H__
#define __RENDER3D_FONTRESOURCE_H__

#include "renderobject.h"
#include <string>

namespace render3d {

	class FontResource : public RenderObject
	{
	public:
		enum FontFlags {
			BOLD = 1,
			ITALIC = 2,
		};

	private:
		enum DirtyFlags {
			FACE_DIRTY = 1,
			SIZE_DIRTY = 2,
			FLAGS_DIRT = 4
		};

		std::string m_face;
		int m_width;
		int m_height;
		unsigned long m_flags;

	public:
		friend class RenderEngine;

		FontResource(RenderEngine *engine, int id, const char *face, int width, int height, unsigned long flags)
			: RenderObject(engine, id) {
			m_width = width;
			m_height = height;
			m_flags = flags;
		}

		void setFace(const char *face) {
			m_face = face;
			m_dirtyFlags |= FACE_DIRTY;
		}

		void setSize(int width, int height) {
			m_width = width;
			m_height = height;
			m_dirtyFlags |= SIZE_DIRTY;
		}

		void setFlags(unsigned long flags) {
			m_flags = flags;
			m_dirtyFlags |= FLAGS_DIRT;
		}

		const std::string face() const { return m_face; }
		int width() const { return m_width; }
		int height() const { return m_height; }
		unsigned long flags() const { return m_flags; }
	};

}

#endif