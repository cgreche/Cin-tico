
#ifndef __CINETICO_H__
#define __CINETICO_H__

#include "cineticodb.h"
#include "cinetico3d.h"
#include <vector>
#include <list>

namespace cinetico {

	class MainWindow;
	class Controller;

	struct View {
		int id;
		const char *name;
		Controller *controller;
	};

	struct BodyPointConfig {
		const char *name;
		int value;

	public:
		BodyPointConfig(const char *name, unsigned long value)
			: name(name), value(value) { }
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

		enum CineticoError {
			SUCCESS,
			USER_ALREADY_EXISTS,
			INVALID_USER_CREDENTIALS,
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
		void goTo(ViewID viewId);
		
		CineticoError createAccount(const char *username, const char *password);
		CineticoError loginUser(const char *username, const char *password);
		int run();

		void enter3DWorld();
		void quit3DWorld();		

		CineticoDB *cineticoDB() const { return m_cineticoDB; }
		Cinetico3D *cinetico3D() const { return m_cinetico3D; }

		UserProfile *currentUser() const { return m_currentUser; }


		std::vector<cinetico::BodyPointConfig *> getAllBodyPointsCaps() {
			std::vector<cinetico::BodyPointConfig *> bodyPointsCaps;

			unsigned long value = 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Cabeça", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Cervical", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Espinha", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Base da espinha", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Ombro esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Cotovelo esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Pulso esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Palma esquerda", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo mínimo esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo anular esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo médio esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo indicador esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo polegar esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Quadril esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Joelho esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Tornozelo esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Pé esquerdo", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Ombro direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Cotovelo direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Pulso direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Palma direita", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo mínimo direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo anular direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo médio direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo indicador direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Dedo polegar direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Quadril direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Joelho direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Tornozelo direito", value));
			value <<= 1;
			bodyPointsCaps.push_back(new cinetico::BodyPointConfig("Pé direito", value));
			return bodyPointsCaps;
		}

		struct BodyPointConfig {
			const char *name;
			int value;
		};

	};
}

#endif