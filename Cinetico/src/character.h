#ifndef __CINETICO_UI_CHARACTER_H__
#define __CINETICO_UI_CHARACTER_H__

#include <vector>
#include "core/lib/vector3.h"
#include "core/lib/quaternion.h"
#include "core/entity/BodyPoint.h"

namespace cinetico {
	using namespace cinetico_core;
	class CineticoUI;

	class BodyPointNode {
		BodyPoint::BodyPart m_bpId;
		BodyPointNode *m_parent;
		std::vector<BodyPointNode *> m_childList;

		cinetico_core::Vector3 m_globalPosition;
		Quaternion m_localOrientation;
		Quaternion m_globalOrientation;

		Matrix4x4 m_localTransform;
		Matrix4x4 m_globalTransform;

	public:
		BodyPointNode(BodyPoint::BodyPart bpId, BodyPointNode* parent) {
			m_bpId = bpId;
			m_parent = parent;
			if (parent)
				parent->addChild(this);
		}

		void addChild(BodyPointNode *node) {
			m_childList.push_back(node);
		}

		void updateState(Quaternion orientation, const Matrix4x4 &transform) {
			m_globalOrientation = m_localOrientation*orientation;
			m_localTransform = m_localOrientation.toRotationMatrix();
			m_globalTransform = m_localTransform*transform;

			Quaternion qTest = Quaternion::fromRotationMatrix(m_globalTransform);

			if (qTest == m_globalOrientation)
				int a = 1;

			m_globalOrientation = qTest;
			for (BodyPointNode *bpn : m_childList)
				bpn->updateState(m_globalOrientation,m_globalTransform);
		}

		void setGlobalPosition(const cinetico_core::Vector3 &position) {
			m_globalPosition = position;
		}

		void setLocalOrientation(const Quaternion &orientation) {
			m_localOrientation = orientation;
		}

		void setGlobalOrientation(const Quaternion &orientation) {
			m_globalOrientation = orientation;
		}

		BodyPoint::BodyPart bp() const { return m_bpId; }
		BodyPointNode *parent() const { return m_parent; }

		const cinetico_core::Vector3 &globalPosition() const { return m_globalPosition; }
		const Quaternion &globalOrientation() const { return m_globalOrientation; }

		Matrix4x4 &localTransform() { return m_localTransform; }
		Matrix4x4 &globalTransform() { return m_globalTransform; }

	};

	class Character {
	protected:
		CineticoUI &m_cineticoUI;

		std::vector<int> m_instanceIds;
		cinetico_core::Vector3 m_position;

		std::vector<BodyPointNode*> m_bodyPoints;
		BodyPointNode *m_rootBodyPoint;

		BodyPointNode *addBodyPointNode(BodyPoint::BodyPart bp, BodyPointNode *parent);
		void createBoneHierarchyTree();

	public:
		Character(CineticoUI &cineticoUI);

		virtual void setPosition(const cinetico_core::Vector3 &position) { m_position = position; }

		virtual void update();
		virtual void render() = 0;
	};
	
}

#endif