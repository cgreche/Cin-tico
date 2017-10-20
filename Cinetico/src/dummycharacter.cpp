
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
	};

	std::vector<int> CineticoResources::ResIdModelMan = std::vector<int>();
	std::vector<int> CineticoResources::ResIdModelWoman = std::vector<int>();
	render3d::Color* CineticoResources::CharacterBodyColors = NULL;

	static bool dummyCharacterLoaded = false;

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
			m_instanceIds.push_back(cineticoUI.renderEngine()->newResourceInstance(CineticoResources::ResIdModelWoman[i]));
			ResourceInstance *modelInstance = cineticoUI.renderEngine()->resourceInstance(m_instanceIds[i]);
			modelInstance->setScale(0.25);
			modelInstance->setPos(render3d::Vector3(0, 5, 2));
		}
	}

	void DummyCharacter::update() {

	}

	void DummyCharacter::render() {
		for (unsigned int i = 0; i < m_instanceIds.size(); ++i)
			m_cineticoUI.renderEngine()->drawResource(m_instanceIds[i]);
	}

}