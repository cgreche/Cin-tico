#ifndef __CINETICO_UI_H__
#define __CINETICO_UI_H__

#include "mainwindow.h"
#include "entity/core/Exercise.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"
#include "bodytracker.h"
#include "cinetico.h"
#include "user-interface/controller.h"

namespace cinetico {

	class DummyCharacter;
	class HumanCharacter;
	class MainWindow;

	using namespace cinetico_core;
	using namespace render3d;

	class render3d::RenderEngine;
	class render3d::RenderEngineHelper;

	struct View {
		int id;
		const char *name;
		Controller *controller;
	};

	class CineticoUI {
	public:
		enum ViewID {
			INVALID = -1,
			LOGIN,
			USER_PROFILE,
			EXERCISES,
			ACTIONS,
			EXERCISE_REALIZATION
		};

	private:
		Cinetico &m_cinetico;
		cinetico_core::Exercise *m_playingExercise;

		void setup();
		void cleanUp();

		void setupWindow();
		void setupRenderEngine();

		MainWindow *m_mainWindow;
		RenderEngine *m_renderEngine;
		RenderEngineHelper *m_renderEngineHelper;

		unsigned int m_globalFrameCount;

		std::vector<View> m_views;
		ViewID m_currentView;

		bool m_headerAndFooterVisible;

		int m_viewport;

	public:
		CineticoUI(Cinetico &cinetico);
		~CineticoUI();

		void registerView(int id, const char *name, Controller *controller);
		void goTo(ViewID viewId, Controller::ViewParams *params = NULL);

		void startExercise(cinetico_core::Exercise &exercise);
		void startPlayground();
		void update();
		void step();
		void render();

		void setHeaderAndFooterVisible(bool visible);

		const Cinetico &cinetico() const { return m_cinetico; }
		MainWindow *mainWindow() const { return m_mainWindow; }
		RenderEngine *renderEngine() const { return m_renderEngine; }
		RenderEngineHelper *renderEngineHelper() const { return m_renderEngineHelper; }
		int viewport() const { return m_viewport; }
	};

}

#endif