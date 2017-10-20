
#include "cinetico.h"
#include "cineticoui.h"
#include "input.h"
#include "ExerciseRealizationController.h"
#include "dummycharacter.h"
#include "humancharacter.h"

#include "playmodes/ExercisePlayMode.h"
#include "playmodes/FreePlayMode.h"

namespace cinetico {

	ExerciseRealizationController::ExerciseRealizationController(Cinetico &cinetico)
		: Controller(cinetico) {
		m_playingMode = NULL;
	}

	Layout *ExerciseRealizationController::viewDefinition() {
		return &layout;
	}

	void ExerciseRealizationController::onViewEnter(ViewParams *params) {

		PlayMode::PlayModeID playModeId = (PlayMode::PlayModeID)(int)(*params)["play_mode"];

		m_renderEngine = m_cinetico.cineticoUI()->renderEngine();
		m_renderEngineHelper = m_cinetico.cineticoUI()->renderEngineHelper();
		m_cinetico.cineticoUI()->setHeaderAndFooterVisible(false);

		PlayMode *playMode = NULL;

		if (playModeId == PlayMode::EXERCISE_MODE) {
			Exercise *exercise = (Exercise*)(*params)["exercise"];
			playMode = new ExercisePlayMode(m_cinetico, *exercise);
		}

		if (playMode)
			playMode->setup();
		m_playingMode = playMode;
	}

	void ExerciseRealizationController::onViewTick() {
		step();
		render();

		bool quit3D = false;
		static bool lastEsc = m_cinetico.input()->keyboard.key(VK_ESCAPE);
		if (m_cinetico.input()->keyboard.key(VK_ESCAPE) == 1 && lastEsc == 0) {
			quit3D = true;
		}

		lastEsc = m_cinetico.input()->keyboard.key(VK_ESCAPE);

		if (quit3D) {
			m_cinetico.cineticoUI()->goTo(CineticoUI::EXERCISES);
			return;
		}
	}

	void ExerciseRealizationController::onViewQuit() {
		m_cinetico.cineticoUI()->setHeaderAndFooterVisible(true);
		if (m_playingMode)
			m_playingMode->cleanUp();
	}

	void ExerciseRealizationController::step() {
		if (m_playingMode)
			m_playingMode->step();
	}

	void ExerciseRealizationController::render() {
		if (m_playingMode) {
			m_renderEngine->beginScene();
			m_playingMode->render();
			m_renderEngine->endScene();
		}
	}

}