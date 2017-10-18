
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

			bodyPointNames.resize(BodyPoint::BodyPartCount);

			bodyPointNames[BodyPoint::Head] = m_dictionary->getString(Dictionary::BodyPointHead);
			bodyPointNames[BodyPoint::Cervical] = m_dictionary->getString(Dictionary::BodyPointCervical);
			bodyPointNames[BodyPoint::Spine] = m_dictionary->getString(Dictionary::BodyPointSpine);
			bodyPointNames[BodyPoint::SpineBase] = m_dictionary->getString(Dictionary::BodyPointSpineBase);
			bodyPointNames[BodyPoint::LeftShoulder] = m_dictionary->getString(Dictionary::BodyPointLeftShoulder);
			bodyPointNames[BodyPoint::LeftElbow] = m_dictionary->getString(Dictionary::BodyPointLeftElbow);
			bodyPointNames[BodyPoint::LeftWrist] = m_dictionary->getString(Dictionary::BodyPointLeftWrist);
			bodyPointNames[BodyPoint::LeftPalm] = m_dictionary->getString(Dictionary::BodyPointLeftPalm);
			bodyPointNames[BodyPoint::LeftPinky] = m_dictionary->getString(Dictionary::BodyPointLeftPinky);
			bodyPointNames[BodyPoint::LeftRingFinger] = m_dictionary->getString(Dictionary::BodyPointLeftRingFinger);
			bodyPointNames[BodyPoint::LeftMiddleFinger] = m_dictionary->getString(Dictionary::BodyPointLeftMiddleFinger);
			bodyPointNames[BodyPoint::LeftIndexFinger] = m_dictionary->getString(Dictionary::BodyPointLeftIndexFinger);
			bodyPointNames[BodyPoint::LeftThumb] = m_dictionary->getString(Dictionary::BodyPointLeftThumb);
			bodyPointNames[BodyPoint::LeftHip] = m_dictionary->getString(Dictionary::BodyPointLeftHip);
			bodyPointNames[BodyPoint::LeftKnee] = m_dictionary->getString(Dictionary::BodyPointLeftKnee);
			bodyPointNames[BodyPoint::LeftAnkle] = m_dictionary->getString(Dictionary::BodyPointLeftAnkle);
			bodyPointNames[BodyPoint::LeftFoot] = m_dictionary->getString(Dictionary::BodyPointLeftFoot);

			bodyPointNames[BodyPoint::RightShoulder] = m_dictionary->getString(Dictionary::BodyPointRightShoulder);
			bodyPointNames[BodyPoint::RightElbow] = m_dictionary->getString(Dictionary::BodyPointRightElbow);
			bodyPointNames[BodyPoint::RightWrist] = m_dictionary->getString(Dictionary::BodyPointRightWrist);
			bodyPointNames[BodyPoint::RightPalm] = m_dictionary->getString(Dictionary::BodyPointRightPalm);
			bodyPointNames[BodyPoint::RightPinky] = m_dictionary->getString(Dictionary::BodyPointRightPinky);
			bodyPointNames[BodyPoint::RightRingFinger] = m_dictionary->getString(Dictionary::BodyPointRightRingFinger);
			bodyPointNames[BodyPoint::RightMiddleFinger] = m_dictionary->getString(Dictionary::BodyPointRightMiddleFinger);
			bodyPointNames[BodyPoint::RightIndexFinger] = m_dictionary->getString(Dictionary::BodyPointRightIndexFinger);
			bodyPointNames[BodyPoint::RightThumb] = m_dictionary->getString(Dictionary::BodyPointRightThumb);
			bodyPointNames[BodyPoint::RightHip] = m_dictionary->getString(Dictionary::BodyPointRightHip);
			bodyPointNames[BodyPoint::RightKnee] = m_dictionary->getString(Dictionary::BodyPointRightKnee);
			bodyPointNames[BodyPoint::RightAnkle] = m_dictionary->getString(Dictionary::BodyPointRightAnkle);
			bodyPointNames[BodyPoint::RightFoot] = m_dictionary->getString(Dictionary::BodyPointRightFoot);
			return bodyPointNames;
		}
	};
}

#endif