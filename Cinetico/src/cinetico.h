
#ifndef __CINETICO_H__
#define __CINETICO_H__

#include "cineticodb.h"
#include "cinetico3d.h"
#include "dictionary.h"
#include "user-interface/controller.h"
#include <vector>
#include <list>

namespace cinetico {

	class MainWindow;

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
			USER_PROFILE,
			EXERCISES,
			ACTIONS,
			EXERCISE_REALIZATION
		};

		enum CineticoError {
			SUCCESS,
			USER_ALREADY_EXISTS,
			INVALID_USER_CREDENTIALS,
		};

	private:
		LanguageID m_currentLangId;
		
		CineticoDB *m_cineticoDB;
		Cinetico3D *m_cinetico3D;
		MainWindow *m_mainWindow;
		Dictionary *m_dictionary;

		std::vector<View> m_views;

		void setup();
		void update();
		void render();
		void cleanUp();

		UserProfile *m_currentUser;
		ViewID m_currentView;

	public:
		Cinetico();
		void registerView(int id, const char *name, Controller *controller);
		void goTo(ViewID viewId, Controller::ViewParams *params = NULL);
		
		CineticoError createAccount(const char *username, const char *password);
		CineticoError loginUser(const char *username, const char *password);
		void logoffCurrentUser();
		int run();
		
		void setLanguage(Dictionary::LanguageID langId);

		MainWindow *mainWindow() const { return m_mainWindow; }
		CineticoDB *cineticoDB() const { return m_cineticoDB; }
		Cinetico3D *cinetico3D() const { return m_cinetico3D; }
		Dictionary *dictionary() const { return m_dictionary; }

		UserProfile *currentUser() const { return m_currentUser; }

		std::vector<uilib::string> getAllBodyPointNames() {
			std::vector<uilib::string> bodyPointNames;

			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointHead));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointCervical));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointSpine));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointSpineBase));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftShoulder));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftElbow));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftWrist));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftPalm));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftPinky));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftRingFinger));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftMiddleFinger));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftIndexFinger));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftThumb));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftHip));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftKnee));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftAnkle));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointLeftFoot));

			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightShoulder));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightElbow));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightWrist));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightPalm));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightPinky));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightRingFinger));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightMiddleFinger));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightIndexFinger));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightThumb));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightHip));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightKnee));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightAnkle));
			bodyPointNames.push_back(m_dictionary->getString(Dictionary::BodyPointRightFoot));
			return bodyPointNames;
		}
	};
}

#endif