
#include "cinetico.h"
#include "cineticoui.h"
#include "input.h"
#include "ExerciseRealizationController.h"
#include "dummycharacter.h"
#include "humancharacter.h"

#include "playmodes/ExercisePlayMode.h"
#include "playmodes/FreePlayMode.h"

namespace cinetico {

	ExerciseRealizationController::ExerciseRealizationController(CineticoUI &cineticoUI)
		: Controller(cineticoUI) {
		m_playingMode = NULL;
	}

	Layout *ExerciseRealizationController::viewDefinition() {
		return &layout;
	}

	void ExerciseRealizationController::onViewEnter(ViewParams *params) {

		PlayMode::PlayModeID playModeId = (PlayMode::PlayModeID)(int)(*params)["play_mode"];

		m_renderEngine = m_cineticoUI.renderEngine();
		m_renderEngineHelper = m_cineticoUI.renderEngineHelper();
		m_cineticoUI.setHeaderAndFooterVisible(false);

		PlayMode *playMode = NULL;

		if (playModeId == PlayMode::EXERCISE_MODE) {
			Exercise *exercise = (Exercise*)(*params)["exercise"];
			playMode = new ExercisePlayMode(m_cinetico, *exercise);
		}

		if (playMode)
			playMode->setup();
		m_playingMode = playMode;

		//Save screen configuration
		Size frameSize = m_cineticoUI.mainWindow()->getFrameSize();
		bool fullscreen = m_cineticoUI.mainWindow()->visibilityMode() == uilib::Fullscreen;
		m_oldWidth = frameSize.width();
		m_oldHeight = frameSize.height();
		m_oldFullscreen = fullscreen;

		m_cineticoUI.setViewResolution(1280, 800, true);
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
			m_cineticoUI.goTo(CineticoUI::EXERCISES);
			return;
		}
	}

	void ExerciseRealizationController::onViewQuit() {
		m_cineticoUI.setHeaderAndFooterVisible(true);
		if (m_playingMode)
			m_playingMode->cleanUp();
		m_cineticoUI.setViewResolution(m_oldWidth, m_oldHeight, m_oldFullscreen);
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