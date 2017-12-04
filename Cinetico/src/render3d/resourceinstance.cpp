
#include "resourceinstance.h"

namespace render3d {

	ResourceInstance::ResourceInstance(RenderEngine *engine, int resId, int resDataId)
		: RenderObject(engine, resId)
		, m_scale(1.f, 1.f, 1.f) {
		m_resDataId = resDataId;
		m_drawFlags = BACKFACE_CULL;
	}

	void ResourceInstance::setPos(const Vector3 &pos) {
		m_pos = pos;
		m_dirtyFlags |= POS_DIRTY;
	}

	void ResourceInstance::setScale(float scale) {
		m_scale.set(scale, scale, scale);
		m_dirtyFlags |= SCALE_DIRTY;
	}

	void ResourceInstance::setScale(const Vector3 &scale) {
		m_scale = scale;
		m_dirtyFlags |= SCALE_DIRTY;
	}

	void ResourceInstance::setRotation(const cinetico_core::Quaternion &rot) {
		m_rot = rot;
		m_dirtyFlags |= ROT_DIRTY;
	}

	void ResourceInstance::setDrawFlags(unsigned long flags) {
		m_drawFlags = flags;
		m_dirtyFlags |= DRAW_FLAGS_DIRTY;
	}

}