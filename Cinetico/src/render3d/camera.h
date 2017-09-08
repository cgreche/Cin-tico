#pragma once

#ifndef __RENDER3D_CAMERA_H__
#define __RENDER3D_CAMERA_H__

#include "renderobject.h"
#include "vector3.h"

namespace render3d {

	class Camera : public RenderObject
	{
	public:
		enum DirtyFlags {
			DIRTY_POS = 1,
			DIRTY_ROT = 2,
			DIRTY_ZOOM = 4
		};

	private:
		Vector3 m_pos; //eye
		Vector3 m_rot;
		float m_zoom;

		Camera(RenderEngine *engine, int id)
			: RenderObject(engine, id) {
			m_zoom = 1.f;
			m_dirtyFlags |= DIRTY_POS | DIRTY_ROT | DIRTY_ZOOM;
		}

		Camera(RenderEngine *engine, int id, const Vector3 &pos, const Vector3 &rot, float zoom)
			: RenderObject(engine, id) {
			m_pos = pos;
			m_rot = rot;
			m_zoom = zoom;
			m_dirtyFlags |= DIRTY_POS | DIRTY_ROT | DIRTY_ZOOM;
		}

	public:
		friend class RenderEngine;
		void setPos(const Vector3 &pos) {
			m_pos = pos;
			m_dirtyFlags |= DIRTY_POS;
		}

		void setRot(const Vector3 &rot) {
			m_rot = rot;
			m_dirtyFlags |= DIRTY_ROT;
		}

		void setZoom(float zoom) {
			m_zoom = zoom;
			m_dirtyFlags |= DIRTY_ZOOM;
		}

		Vector3 pos() { return m_pos; }
		Vector3 rot() { return m_rot; }
		float zoom() { return m_zoom; }
	};

}

#endif