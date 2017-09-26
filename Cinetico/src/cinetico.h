
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
		MainWindow *m_mainWindow;

		std::vector<View> m_views;
		bool m_onWorld3D;

		void setup();
		void update();
		void render();
		void cleanUp();

		ViewID m_currentView;

	public:
		Cinetico();
		void registerView(int id, const char *name, Controller *controller);
		int run();
		void goTo(ViewID viewId);
		void setOnWorld3D(bool onWorld3D) { m_onWorld3D = onWorld3D; }
	};
}