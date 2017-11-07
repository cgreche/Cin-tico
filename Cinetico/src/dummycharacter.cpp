
#include "cinetico.h"
#include "cineticoui.h"
#include "dummycharacter.h"

namespace cinetico {

	using namespace render3d;

#define CM2W 2
#define GRAY_COLOR 80

	class CineticoResources
	{
	public:
		static std::vector<int> ResIdModelMan;
		static std::vector<int> ResIdModelWoman;
		static render3d::Color *CharacterBodyColors;
		static int ResIdBB;
		static int InstBB;
	};

	std::vector<int> CineticoResources::ResIdModelMan = std::vector<int>();
	std::vector<int> CineticoResources::ResIdModelWoman = std::vector<int>();
	render3d::Color* CineticoResources::CharacterBodyColors = NULL;

	int CineticoResources::ResIdBB;
	int CineticoResources::InstBB;

	static bool dummyCharacterLoaded = false;

	static Box bb;

	DummyCharacter::DummyCharacter(CineticoUI &cineticoUI)
		: Character(cineticoUI) {
		unsigned int i;

		if (!dummyCharacterLoaded) {
			CineticoResources::ResIdModelWoman = cineticoUI.renderEngineHelper()->loadModel("MODEL.dae");
			/*
			for (unsigned int i = 0; i < CineticoResources::ResIdModelWoman.size(); ++i) {
				ResourceData *model = cinetico3d.renderEngine()->resourceData(CineticoResources::ResIdModelWoman[i]);
				Color *modelColors = new Color[model->vertexCount()];
				for (int i = 0; i < model->vertexCount(); ++i)
					modelColors[i] = Color(GRAY_COLOR, GRAY_COLOR, GRAY_COLOR);
				model->setColors(modelColors);
				delete[] modelColors;
			}
			*/

			dummyCharacterLoaded = true;
		}

		for (i = 0; i < CineticoResources::ResIdModelWoman.size(); ++i) {
			ResourceData *data = cineticoUI.renderEngine()->resourceData(CineticoResources::ResIdModelWoman[i]);
			Box box = data->getBoundingBox();
			bb.unite(box);
			
			m_instanceIds.push_back(cineticoUI.renderEngine()->newResourceInstance(CineticoResources::ResIdModelWoman[i]));
			ResourceInstance *modelInstance = cineticoUI.renderEngine()->resourceInstance(m_instanceIds[i]);
			//modelInstance->setScale(0.25);
			
		}

		CineticoResources::ResIdBB = cineticoUI.renderEngineHelper()->createRectangularPrism(bb.size().x(), bb.size().y(), bb.size().z());
		CineticoResources::InstBB = cineticoUI.renderEngine()->newResourceInstance(CineticoResources::ResIdBB);
		cineticoUI.renderEngine()->resourceInstance(CineticoResources::InstBB)->setDrawFlags(ResourceInstance::WIREFRAME);
	}

	void DummyCharacter::update() {
		for (unsigned int i = 0; i < CineticoResources::ResIdModelWoman.size(); ++i) {
			ResourceInstance *modelInstance = m_cineticoUI.renderEngine()->resourceInstance(m_instanceIds[i]);
			render3d::Vector3 modelPos = render3d::Vector3(m_position.x(), m_position.y()-bb.y2()+bb.size().y(), m_position.z());
			modelInstance->setPos(modelPos);
		}
		m_cineticoUI.renderEngine()->resourceInstance(CineticoResources::InstBB)->setPos(render3d::Vector3(m_position.x(),m_position.y()+bb.size().y()/2,m_position.z()));
	}

	void DummyCharacter::render() {
		for (unsigned int i = 0; i < m_instanceIds.size(); ++i) {
			m_cineticoUI.renderEngine()->drawResource(m_instanceIds[i]);
			m_cineticoUI.renderEngine()->drawResource(CineticoResources::InstBB);
		}
	}

}