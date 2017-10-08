#pragma once

#ifndef __RENDER3D_RESOURCEDATA_H__
#define __RENDER3D_RESOURCEDATA_H__

#include "color.h"
#include "vector3.h"
#include "renderobject.h"

namespace render3d {

	class RenderEngine;

	class ResourceData : public RenderObject
	{
	public:
		enum DirtyFlags {
			VERTICES_DIRTY = 0x01,
			INDICES_DIRTY = 0x02,
			NORMALS_DIRTY = 0x04,
			COLORS_DIRTY = 0x08
		};

	private:

		int m_vertexCount;
		Vector3 *m_vertices;
		Vector3 *m_normals;
		Color *m_colors;
		int m_indexCount;
		int *m_indices;

		ResourceData(RenderEngine *engine, int resId);

	public:
		friend class RenderEngine;

		~ResourceData();

		void setVertices(unsigned int vertexCount, Vector3 vertices[]);
		void setIndices(unsigned int indexCount, int indices[]);
		void setNormals(Vector3 normals[]);
		void setColors(Color *colors);

		int vertexCount() const { return m_vertexCount; }
		Vector3 *vertices() const { return m_vertices; }
		int indexCount() const { return m_indexCount; }
		int *indices() const { return m_indices; }
		Vector3 *normals() const { return m_normals; }
		Color *colors() const { return m_colors; }
	};

}

#endif