#pragma once

#ifndef __RENDER3D_RESOURCEINSTANCE_H__
#define __RENDER3D_RESOURCEINSTANCE_H__

#include "color.h"
#include "vertex3.h"
#include "vector3.h"
#include "renderobject.h"
#include "resourcedata.h"

namespace render3d {

	class RenderEngine;

	class ResourceInstance : public RenderObject
	{
	public:
		enum DirtyFlags {
			POS_DIRTY = 0x01,
			ROT_DIRTY = 0x02,
			SCALE_DIRTY = 0x04
		};

	private:
		int m_resDataId;
		Vector3 m_pos;
		Vector3 m_scale;
		Vector3 m_rot;

		ResourceInstance(RenderEngine *engine, int resId, int resDataId);

	public:
		friend class RenderEngine;
		void setPos(const Vector3 &pos);
		void setScale(float scale);
		void setScale(const Vector3 &scale);
		void setRot(const Vector3 &rot);

		int resDataId() const { return m_resDataId; }
		Vector3 pos() const { return m_pos; }
		Vector3 scale() const { return m_scale; }
		Vector3 rot() const { return m_rot; }
	};

}

#endif