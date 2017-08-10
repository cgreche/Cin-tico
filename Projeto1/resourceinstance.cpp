
#include "resourceinstance.h"

ResourceInstance::ResourceInstance(RenderEngine *engine, int resId, int resDataId)
: RenderObject(engine,resId)
, m_scale(1.f,1.f,1.f) {
	m_resDataId = resDataId;
}

void ResourceInstance::setPos(const Vector3 &pos) {
	m_pos = pos;
	m_dirtyFlags |= POS_DIRTY;
}

void ResourceInstance::setScale(float scale) {
	m_scale.set(scale,scale,scale);
	m_dirtyFlags |= SCALE_DIRTY;
}

void ResourceInstance::setScale(const Vector3 &scale) {
	m_scale = scale;
	m_dirtyFlags |= SCALE_DIRTY;
}

void ResourceInstance::setRot(const Vector3 &rot) {
	m_rot = rot;
	m_dirtyFlags |= ROT_DIRTY;
}