
#include "renderengine.h"
#include "resourcedata.h"

#include <cstring>

namespace render3d {

	ResourceData::ResourceData(RenderEngine *engine, int resId)
		: RenderObject(engine, resId)
	{
		m_vertices = NULL;
		m_indices = NULL;
		m_colors = NULL;
		m_vertexCount = 0;
		m_indexCount = 0;
		m_colors = NULL;
	}

	ResourceData::~ResourceData()
	{
		if (m_vertices)
			delete[] m_vertices;
		if (m_indices)
			delete[] m_indices;
		if (m_colors)
			delete[] m_colors;
	}

	void ResourceData::setVertices(unsigned int vertexCount, Vector3 vertices[]) {
		if (m_vertices)
			delete[] m_vertices;

		m_vertices = new Vector3[vertexCount];
		::memcpy(m_vertices, vertices, vertexCount * sizeof(Vector3));
		m_vertexCount = vertexCount;

		m_dirtyFlags |= VERTEX_DIRTY;
	}

	void ResourceData::setIndices(unsigned int indexCount, int indices[]) {
		if (m_indices)
			delete[] m_indices;

		m_indices = new int[indexCount];
		::memcpy(m_indices, indices, indexCount * sizeof(int));
		m_indexCount = indexCount;

		m_dirtyFlags |= INDEX_DIRTY;
	}

	void ResourceData::setColors(Color *colors) {
		if (m_colors)
			delete[] m_colors;

		m_colors = new Color[m_vertexCount];
		::memcpy(m_colors, colors, m_vertexCount * sizeof(Color));

		m_dirtyFlags |= COLOR_DIRTY;
	}

}