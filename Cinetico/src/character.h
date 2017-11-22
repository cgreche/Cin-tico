#ifndef __CINETICO_UI_CHARACTER_H__
#define __CINETICO_UI_CHARACTER_H__

#include <vector>
#include "entity/core/vector3.h"
#include "entity/core/BodyPoint.h"

namespace cinetico {
	using namespace cinetico_core;
	class CineticoUI;

	class BodyPointNode {
		BodyPoint::BodyPart m_bpId;
		BodyPointNode *m_parent;
		std::vector<BodyPointNode *> m_childList;

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

	};

	class Character {
	protected:
		CineticoUI &m_cineticoUI;

		std::vector<int> m_instanceIds;
		cinetico_core::Vector3 m_position;

		void createBoneHierarchyTree();
	public:
		Character(CineticoUI &cineticoUI);

		virtual void setPosition(const cinetico_core::Vector3 &position) { m_position = position; }

		virtual void update() = 0;
		virtual void render() = 0;
	};
	
}

#endif