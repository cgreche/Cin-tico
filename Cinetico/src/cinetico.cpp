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
#include "utils/crypter.h"

#define INITIAL_VIEW Cinetico::EXERCISE_MANAGEMENT

namespace cinetico {

	Cinetico g_cinetico;
	
	Cinetico::Cinetico() {
		m_onWorld3D = false;
		m_currentView = INVALID;
		m_currentUser = NULL;
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
		m_mainWindow = new MainWindow(*this);

#if 1
		goTo(INITIAL_VIEW);
#else
		enter3DWorld();
#endif
	}


	void Cinetico::update()
	{
		m_mainWindow->update();
		if (m_currentView != INVALID) {
			Controller *currentController = m_views[m_currentView].controller;
			if (currentController) {
				currentController->onViewTick();
			}
		}
	}


	void Cinetico::render()
	{
	}

	void Cinetico::cleanUp() {
		for (unsigned int i = 0; i < m_views.size(); ++i) {
			delete m_views[i].controller;
		}

//		if (m_mainWindow)
//			delete m_mainWindow;
		if(m_cinetico3D)
			delete m_cinetico3D;
		if(m_cineticoDB)
			delete m_cineticoDB;
	}

	Cinetico::CineticoError Cinetico::createAccount(const char *username, const char *password) {

		UserProfile *user = cineticoDB()->userProfileDAO()->getByLoginName(username);
		if(user) {
			delete user;
			return USER_ALREADY_EXISTS;
		}

		//Account creation
		std::string cryptPW = Crypter::SimpleHash(password);
		user = new UserProfile(username, cryptPW);
		cineticoDB()->userProfileDAO()->save(*user);

		return SUCCESS;
	}

	Cinetico::CineticoError Cinetico::loginUser(const char *username, const char *password) {
		UserProfile *user = m_cineticoDB->userProfileDAO()->getByLoginName(username);
		if (user) {
			std::string cryptPW = Crypter::SimpleHash(password);
			if (user->login(cryptPW)) {
				m_currentUser = user;
				return this->SUCCESS;
			}
			delete user;
		}
		return this->INVALID_USER_CREDENTIALS;
	}

	void Cinetico::logoffCurrentUser() {
		if (m_currentUser) {
			m_currentUser->logoff();
			m_currentUser = NULL;
		}
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

	void Cinetico::goTo(ViewID viewId, Controller::ViewParams *params) {
		if (m_currentView != INVALID) {
			Controller *currentController = m_views[m_currentView].controller;
			currentController->onViewQuit();
		}

		Controller *controller = m_views[viewId].controller;
		m_mainWindow->setContentLayout(controller->viewDefinition());
		m_currentView = viewId;
		controller->onViewEnter(params);

		m_mainWindow->setSize(Size(1024, 768));
	}

	void Cinetico::enter3DWorld() {
		m_mainWindow->setVisible(false);
		m_onWorld3D = true;
	}

	void Cinetico::quit3DWorld() {
		goTo(EXERCISES);
		m_mainWindow->setVisible(true);
		m_onWorld3D = false;
	}

}

using namespace cinetico;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	return g_cinetico.run();
}