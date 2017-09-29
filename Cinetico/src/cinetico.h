
#ifndef __CINETICO_H__
#define __CINETICO_H__

#include "cineticodb.h"
#include "cinetico3d.h"
#include <vector>

namespace cinetico {

	class MainWindow;
	class Controller;

	struct View {
		int id;
		const char *name;
		Controller *controller;
	};

	class Cinetico {

	public:
		enum ViewID {
			INVALID = -1,
			LOGIN,
			EXERCISES,
			EXERCISE_MANAGEMENT,
			EXERCISE_REALIZATION
		};

	private:
		CineticoDB *m_cineticoDB;
		Cinetico3D *m_cinetico3D;
		MainWindow *m_mainWindow;

		std::vector<View> m_views;
		bool m_onWorld3D;

		void setup();
		void update();
		void render();
		void cleanUp();

		UserProfile *m_currentUser;
		ViewID m_currentView;

	public:
		Cinetico();
		void registerView(int id, const char *name, Controller *controller);
		int run();
		void goTo(ViewID viewId);
		
		void enter3DWorld();
		void quit3DWorld();

		void setUser(UserProfile *user) { m_currentUser = user; }

		CineticoDB *cineticoDB() const { return m_cineticoDB; }
		Cinetico3D *cinetico3D() const { return m_cinetico3D; }

		UserProfile *currentUser() const { return m_currentUser; }
	};
}

#endif