#pragma once

#ifndef __RENDER3D_RESOURCEINSTANCE_H__
#define __RENDER3D_RESOURCEINSTANCE_H__

#include "color.h"
#include "vector3.h"
#include "renderobject.h"
#include "resourcedata.h"
#include "core/lib/quaternion.h"

namespace render3d {

	class RenderEngine;

	class ResourceInstance : public RenderObject
	{
	public:
		enum DirtyFlags {
			POS_DIRTY = 0x01,
			ROT_DIRTY = 0x02,
			SCALE_DIRTY = 0x04,
			DRAW_FLAGS_DIRTY = 0x08
		};

		enum DrawFlags {
			BACKFACE_CULL = 1,
			WIREFRAME = 2
		};

	private:
		int m_resDataId;
		Vector3 m_pos;
		Vector3 m_scale;
		cinetico_core::Quaternion m_rot;
		unsigned long m_drawFlags;

		ResourceInstance(RenderEngine *engine, int resId, int resDataId);

	public:
		friend class RenderEngine;
		void setPos(const Vector3 &pos);
		void setScale(float scale);
		void setScale(const Vector3 &scale);
		void setRotation(const cinetico_core::Quaternion &rot);
		void setDrawFlags(unsigned long flags);

		int resDataId() const { return m_resDataId; }
		Vector3 pos() const { return m_pos; }
		Vector3 scale() const { return m_scale; }
		cinetico_core::Quaternion rot() const { return m_rot; }
		unsigned long drawFlags() const { return m_drawFlags; }
	};

}

#endif