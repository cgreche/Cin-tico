
#include "cinetico.h"
#include "cineticoui.h"
#include "core/entity/Exercise.h"
#include "render3d/d3d9engine.h"

#include "user-interface/mainwindow.h"
#include "user-interface/Controller.h"
#include "user-interface/LoginController.h"
#include "user-interface/UserProfileController.h"
#include "user-interface/ExercisesController.h"
#include "user-interface/ActionsController.h"
#include "user-interface/ExerciseRealizationController.h"
#include "user-interface/GeneralConfigController.h"
#include "user-interface/DebugController.h"

using namespace cinetico_core;
using namespace render3d;

namespace cinetico {

	float aspectRatio43 = 1.33334f; //4:3
	float aspectRatio51 = 1.25; //5:4
	float aspectRatio169 = 1.77778f; //16:9

	float currentAspectRatio = aspectRatio169;

	int winWidth = 1280;
	int winHeight = (int)ceil(winWidth / currentAspectRatio);

	class RenderEngineFactory {
	public:
		static RenderEngine *getRenderEngine() {
			return new D3D9Engine();
		}
	};

	CineticoUI::CineticoUI(Cinetico &cinetico)
		: m_cinetico(cinetico) {
		m_currentView = INVALID;
		m_lastView = INVALID;
		m_mainWindow = NULL;
		m_playingExercise = NULL;
		m_globalFrameCount = 0;
		setup();
	}

	CineticoUI::~CineticoUI() {
		cleanUp();
	}

	void CineticoUI::registerView(int id, const char *name, Controller *controller) {
		m_views.push_back({ id,name,controller });
	}

	void CineticoUI::setupWindow() {
		m_mainWindow = new MainWindow(m_cinetico);
		m_mainWindow->setFrameSize(Size(winWidth, winHeight));
	}

	void CineticoUI::setupRenderEngine()
	{
		m_renderEngine = RenderEngineFactory::getRenderEngine();
		m_renderEngine->configure(m_mainWindow->osdRef().handle());
		m_renderEngineHelper = new RenderEngineHelper(*m_renderEngine);
		int width = m_renderEngine->config().displaymode().width;
		int height = m_renderEngine->config().displaymode().height;
		bool fullscreen = m_renderEngine->config().fullscreen();
		m_viewport = m_renderEngine->newViewport(0, 0, width, height);
	}

	void CineticoUI::setup() {

		Dictionary *dictionary = m_cinetico.dictionary();
		//Setup views
		registerView(LOGIN, dictionary->getString(Dictionary::LoginViewTitle).data(), new LoginController(*this));
		registerView(USER_PROFILE, dictionary->getString(Dictionary::UserProfileViewTitle).data(), new UserProfileController(*this));
		registerView(EXERCISES, dictionary->getString(Dictionary::ExercisesViewTitle).data(), new ExercisesController(*this));
		registerView(ACTIONS, dictionary->getString(Dictionary::ActionsViewTitle).data(), new ActionsController(*this));
		registerView(PLAYING, "Exercise Realization", new ExerciseRealizationController(*this));
		registerView(GENERAL_CONFIG, "Configurações Gerais", new GeneralConfigController(*this));
		registerView(DEBUG, "Debug mode", new DebugController(*this));

		setupWindow();
		setupRenderEngine();
	}

	void CineticoUI::goTo(ViewID viewId, Controller::ViewParams params) {
		m_lastView = m_currentView;
		if (m_currentView != INVALID) {
			Controller *currentController = m_views[m_currentView].controller;
			currentController->onViewQuit();
		}

		Controller *controller = m_views[viewId].controller;
		controller->onViewUpdate();
		m_mainWindow->setContentLayout(controller->viewDefinition());
		m_currentView = viewId;
		params.set<ViewID>("last_view", m_lastView);
		controller->onViewEnter(params);

		m_mainWindow->setSize(m_mainWindow->size());
		m_mainWindow->redraw();
	}

	void CineticoUI::cleanUp() {

		for (unsigned int i = 0; i < m_views.size(); ++i) {
			delete m_views[i].controller;
		}

		if (m_renderEngineHelper)
			delete m_renderEngineHelper;
		if (m_renderEngine)
			delete m_renderEngine;
	}

	void CineticoUI::update() {
		if (m_currentView != INVALID) {
			Controller *controller = m_views[m_currentView].controller;
			controller->onViewUpdate();
		}

		//update Window
		m_mainWindow->update();
	}

	void CineticoUI::step() {
		++m_globalFrameCount;
		m_mainWindow->step();
		if (m_currentView != INVALID) {
			Controller *currentController = m_views[m_currentView].controller;
			if (currentController) {
				currentController->onViewTick();
			}
		}
	}

	void CineticoUI::setViewResolution(int width, int height, bool fullscreen) {
		Viewport *viewport = m_renderEngine->viewport(m_viewport);
		viewport->setSize(width, height);
		m_mainWindow->setVisibilityMode(fullscreen ? uilib::Fullscreen : uilib::ShowNormal);
		m_mainWindow->setFrameSize(Size(width, height));
		
	}

	void CineticoUI::setHeaderContentFooterVisible(bool headerVisible, bool contentVisible, bool footerVisible) {
		m_mainWindow->setHeaderVisible(headerVisible);
		m_mainWindow->setContentVisible(contentVisible);
		m_mainWindow->setFooterVisible(footerVisible);
		update();
	}

}