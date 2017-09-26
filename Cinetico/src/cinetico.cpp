#include <windows.h>
#include <math.h>
#include <time.h>

#include "cinetico.h"
#include "mainwindow.h"
#include "user-interface/LoginController.h"
#include "user-interface/ExercisesController.h"
#include "user-interface/ExerciseManagementController.h"
#include "user-interface/ExerciseRealizationController.h"

namespace cinetico {
	//todo: get rid of externs
	extern void setupWorld3D();
	extern void updateWorld3D();
	extern void renderWorld3D();
	extern void destroyWorld3D();

	Cinetico g_cinetico;
	
	Cinetico::Cinetico() {
		m_onWorld3D = false;
		m_currentView = INVALID;
	}

	void Cinetico::registerView(int id, const char *name, Controller *controller) {
		m_views.push_back({ id,name,controller });
	}

	void Cinetico::setup()
	{
		//Setup views
		registerView(LOGIN, "Login", new LoginController());
		registerView(EXERCISES, "Exercises", new ExercisesController());
		registerView(EXERCISE_MANAGEMENT, "Exercise Management", new ExerciseManagementController());
		registerView(EXERCISE_REALIZATION, "Exercise Realization", new ExerciseRealizationController());
		
		m_cineticoDB = new CineticoDB(*this);
		m_mainWindow = new MainWindow();

#if 1
		goTo(LOGIN);
#else
		setupWorld3D();
		m_onWorld3D = true;
#endif
	}


	void Cinetico::update()
	{
		if (m_onWorld3D) {
			updateWorld3D();
		}
	}


	void Cinetico::render()
	{
		if (m_onWorld3D) {
			renderWorld3D();
		}
	}

	void Cinetico::cleanUp() {
		for (unsigned int i = 0; i < m_views.size(); ++i) {
			delete m_views[i].controller;
		}

		delete m_cineticoDB;
		destroyWorld3D();
	}


	int Cinetico::run() {
		setup();

		while (uibase::UIProcess()) {
			update();
			render();
		}

		if (!m_onWorld3D) {
		}

		cleanUp();
		return 0;
	}

	void Cinetico::goTo(ViewID viewId) {
		if (m_currentView != INVALID) {
			Controller *currentController = m_views[m_currentView].controller;
			currentController->onViewQuit();
		}

		Controller *controller = m_views[viewId].controller;
		m_mainWindow->setContentLayout(controller->viewDefinition());
		m_currentView = viewId;
		controller->onViewEnter();

		m_mainWindow->setSize(Size(1024, 768));
	}

}

using namespace cinetico;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return g_cinetico.run();
}