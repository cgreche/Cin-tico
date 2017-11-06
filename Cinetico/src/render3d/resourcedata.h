#pragma once

#ifndef __RENDER3D_RESOURCEDATA_H__
#define __RENDER3D_RESOURCEDATA_H__
#undef min
#undef max

#include "color.h"
#include "vector3.h"
#include "renderobject.h"
#include <algorithm>

namespace render3d {

	class RenderEngine;

	class Box {
		float m_x1, m_x2, m_y1, m_y2, m_z1, m_z2;

	public:
		Box() {
			m_x1 = m_x2 = m_y1 = m_y2 = m_z1 = m_z2 = 0.f;
		}

		Box(float x1, float x2, float y1, float y2, float z1, float z2) {
			m_x1 = x1;
			m_x2 = x2;
			m_y1 = y1;
			m_y2 = y2;
			m_z1 = z1;
			m_z2 = z2;
		}

		Vector3 center() {
			return Vector3((m_x2 + m_x1)*0.5f, (m_y2 + m_y1)*0.5f, (m_z2 + m_z1)*0.5f);
		}

		Vector3 size() {
			return Vector3((m_x2 - m_x1), (m_y2 - m_y1), (m_z2 - m_z1));
		}

		Box& unite(const Box& box) {
			m_x1 = std::min(m_x1, box.x1());
			m_x2 = std::max(m_x2, box.x2());
			m_y1 = std::min(m_y1, box.y1());
			m_y2 = std::max(m_y2, box.y2());
			m_z1 = std::min(m_z1, box.z1());
			m_z2 = std::max(m_z2, box.z2());
			return *this;
		}

		Box unionWith(const Box& box) const {
			float x1 = std::min(m_x1, box.x1());
			float x2 = std::max(m_x2, box.x2());
			float y1 = std::min(m_y1, box.y1());
			float y2 = std::max(m_y2, box.y2());
			float z1 = std::min(m_z1, box.z1());
			float z2 = std::max(m_z2, box.z2());
			return Box(x1, x2, y1, y2, z1, z2);
		}

		void setX1(float x1) { m_x1 = x1; }
		void setX2(float x2) { m_x2 = x2; }
		void setY1(float y1) { m_y1 = y1; }
		void setY2(float y2) { m_y2 = y2; }
		void setZ1(float z1) { m_z1 = z1; }
		void setZ2(float z2) { m_z2 = z2; }

		float x1() const { return m_x1; }
		float x2() const { return m_x2; }
		float y1() const { return m_y1; }
		float y2() const { return m_y2; }
		float z1() const { return m_z1; }
		float z2() const { return m_z2; }

	};

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

		Box getBoundingBox();

		int vertexCount() const { return m_vertexCount; }
		Vector3 *vertices() const { return m_vertices; }
		int indexCount() const { return m_indexCount; }
		int *indices() const { return m_indices; }
		Vector3 *normals() const { return m_normals; }
		Color *colors() const { return m_colors; }
	};

}

#endif