#ifndef __CINETICO_UI_H__
#define __CINETICO_UI_H__

#include "user-interface/mainwindow.h"
#include "user-interface/controller.h"
#include "entity/core/Exercise.h"
#include "render3d/renderengine.h"
#include "render3d/renderenginehelper.h"
#include "bodytracker.h"
#include "cinetico.h"

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
			PLAYING
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
		ViewID m_lastView;

		bool m_headerAndFooterVisible;

		int m_viewport;

	public:
		CineticoUI(Cinetico &cinetico);
		~CineticoUI();

		void registerView(int id, const char *name, Controller *controller);
		void goTo(ViewID viewId, Controller::ViewParams params = Controller::ViewParams());

		void update();
		void step();

		void setViewResolution(int width, int height, bool fullscreen);
		void setHeaderAndFooterVisible(bool visible);

		Cinetico &cinetico() const { return m_cinetico; }
		MainWindow *mainWindow() const { return m_mainWindow; }
		RenderEngine *renderEngine() const { return m_renderEngine; }
		RenderEngineHelper *renderEngineHelper() const { return m_renderEngineHelper; }
		int viewport() const { return m_viewport; }
		ViewID currentView() const { return m_currentView; }
		ViewID lastView() const { return m_lastView; }
	};

}

#endif