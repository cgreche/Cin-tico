#pragma once

#ifndef __RENDER3D_RESOURCEDATA_H__
#define __RENDER3D_RESOURCEDATA_H__

#include "color.h"
#include "vertex3.h"
#include "vector3.h"
#include "renderobject.h"

namespace render3d {

	class RenderEngine;

#define VERTEX_DIRTY 0x01
#define INDEX_DIRTY 0x02
#define COLOR_DIRTY 0x04

	class ResourceData : public RenderObject
	{
		int m_vertexCount;
		Vertex3 *m_vertices;
		Color *m_colors;
		int m_indexCount;
		int *m_indices;

		ResourceData(RenderEngine *engine, int resId);

	public:
		friend class RenderEngine;

		~ResourceData();

		void setVertices(unsigned int vertexCount, Vertex3 vertices[]);
		void setIndices(unsigned int indexCount, int indices[]);
		void setColors(Color *colors);

		int vertexCount() const { return m_vertexCount; }
		Vertex3 *vertices() const { return m_vertices; }
		int indexCount() const { return m_indexCount; }
		int *indices() const { return m_indices; }
		Color *colors() const { return m_colors; }
	};

}

#endif