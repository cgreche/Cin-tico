
#include "renderengine.h"
#include "resourcedata.h"

ResourceData::ResourceData(RenderEngine *engine, int resId)
: RenderObject(engine,resId)
{
	m_vertexCount = 0;
	m_indexCount = 0;
	m_colors = NULL;
}

void ResourceData::setVertices(unsigned int vertexCount, Vertex3 vertices[]) {
	m_vertexCount = vertexCount;
	m_vertices = vertices;
	m_dirtyFlags |= VERTEX_DIRTY;
}

void ResourceData::setIndices(unsigned int indexCount, int indices[]) {
	m_indexCount = indexCount;
	m_indices = indices;
	m_dirtyFlags |= INDEX_DIRTY;
}

void ResourceData::setColors(Color *colors) {
	m_colors = colors;
	m_dirtyFlags |= COLOR_DIRTY;
}

