#include <windows.h>
#include <math.h>
#include <time.h>

#include "cinetico.h"
#include "cineticodb.h"
#include "cinetico3d.h"
#include "mainwindow.h"
#include "user-interface/LoginController.h"
#include "user-interface/ExercisesController.h"
#include "user-interface/ExerciseManagementController.h"
#include "user-interface/ExerciseRealizationController.h"

namespace cinetico {

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
		m_cinetico3D = new Cinetico3D(*this);
		m_mainWindow = new MainWindow();

#if 0
		goTo(LOGIN);
#else
		enter3DWorld();
#endif
	}


	void Cinetico::update()
	{
		if (m_onWorld3D) {
			m_cinetico3D->update();
		}
	}


	void Cinetico::render()
	{
		if (m_onWorld3D) {
			m_cinetico3D->render();
		}
	}

	void Cinetico::cleanUp() {
		for (unsigned int i = 0; i < m_views.size(); ++i) {
			delete m_views[i].controller;
		}

		if(m_cinetico3D)
			delete m_cinetico3D;
		if(m_cineticoDB)
			delete m_cineticoDB;
	}


	int Cinetico::run() {
		setup();

		while (uibase::UIProcess()) {
			update();
			render();
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

	void Cinetico::enter3DWorld() {
		m_mainWindow->setVisible(false);
		m_onWorld3D = true;
	}

}

using namespace cinetico;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return g_cinetico.run();
}