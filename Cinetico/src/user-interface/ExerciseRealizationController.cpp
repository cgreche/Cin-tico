
#include "cinetico.h"
#include "cineticoui.h"
#include "input.h"
#include "ExerciseRealizationController.h"
#include "dummycharacter.h"
#include "humancharacter.h"

#include "playmodes/ExercisePlayMode.h"
#include "playmodes/FreePlayMode.h"
#include "playmodes/DebugPlayMode.h"

namespace cinetico {

	ExerciseRealizationController::ExerciseRealizationController(CineticoUI &cineticoUI)
		: Controller(cineticoUI) {
		m_playingMode = NULL;
	}

	Layout *ExerciseRealizationController::viewDefinition() {
		return &layout;
	}

	void ExerciseRealizationController::onViewEnter(ViewParams params) {

		PlayMode::PlayModeID playModeId = (PlayMode::PlayModeID)params.get<int>("play_mode");

		//Save screen configuration
		Size frameSize = m_cineticoUI.mainWindow()->getFrameSize();
		bool fullscreen = m_cineticoUI.mainWindow()->visibilityMode() == uilib::Fullscreen;
		m_oldWidth = frameSize.width();
		m_oldHeight = frameSize.height();
		m_oldFullscreen = fullscreen;

		m_renderEngine = m_cineticoUI.renderEngine();
		m_renderEngineHelper = m_cineticoUI.renderEngineHelper();
		RenderEngine::Config &config = m_renderEngine->config();
		m_cineticoUI.setViewResolution(config.displaymode().width, config.displaymode().height, config.fullscreen());
		m_cineticoUI.setHeaderContentFooterVisible(false,false,false);
		m_renderEngine->init();

		PlayMode *playMode = NULL;

		if (playModeId == PlayMode::EXERCISE_MODE) {
			Exercise *exercise = params.get<Exercise*>("exercise");
			playMode = new ExercisePlayMode(m_cinetico, *exercise);
		}
		else if (playModeId == PlayMode::DEBUG_MODE) {
			playMode = new DebugPlayMode(m_cinetico);
		}

		if (playMode)
			playMode->setup();
		m_playingMode = playMode;

		m_cinetico.sensor()->initialize();
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
			m_cineticoUI.goTo(m_cineticoUI.lastView());
			return;
		}
	}

	void ExerciseRealizationController::onViewQuit() {
		m_cineticoUI.setHeaderContentFooterVisible(true,true,true);
		if (m_playingMode)
			m_playingMode->cleanUp();
		m_renderEngine->destroy();
		m_cinetico.sensor()->finalize();
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