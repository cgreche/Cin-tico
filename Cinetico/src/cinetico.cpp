
#include "cinetico.h"
#include "cineticodb.h"
#include "cineticoui.h"
#include "input.h"
#include "utils/crypter.h"
#include "kinectsensor.h"
#include "uilib/lib/time.h"

namespace cinetico {

	using namespace uilib;

	class SensorFactory
	{
	public:
		static Sensor *getSystemSensor() {
#ifdef WIN32
			return new KinectSensor();
#endif
		}
	};

	Cinetico::Cinetico() {
		m_currentTime = 0;
		m_sensor = NULL;
		m_bodyTracker = NULL;
		m_currentUser = NULL;
	}

	void Cinetico::setup()
	{
		m_input = new Input();

		m_sensor = SensorFactory::getSystemSensor();
		m_sensor->initialize();
		m_bodyTracker = new BodyTracker(*m_sensor);

		m_dictionary = new Dictionary(*this);

		m_cineticoDB = new CineticoDB(*this);
		m_cineticoUI = new CineticoUI(*this);
		

		m_cineticoUI->goTo(CineticoUI::LOGIN, NULL);
	}


	void Cinetico::step()
	{
		m_currentTime = OSTime::ticks();
		m_cineticoUI->step();
	}

	void Cinetico::cleanUp() {

		if (m_input)
			delete m_input;

		if (m_bodyTracker)
			delete m_bodyTracker;

		if (m_sensor) {
			m_sensor->finalize();
			delete m_sensor;
		}

		if(m_cineticoUI)
			delete m_cineticoUI;
		if(m_cineticoDB)
			delete m_cineticoDB;
		if (m_dictionary)
			delete m_dictionary;
	}

	Cinetico::CineticoError Cinetico::createAccount(const char *username, const char *password) {

		UserProfile *user = cineticoDB()->userProfileDAO()->getByLoginName(username);
		if(user) {
			delete user;
			return USER_ALREADY_EXISTS;
		}

		//Account creation
		std::string cryptPW = Crypter::SimpleHash(password);
		user = new UserProfile(username, cryptPW, "");
		cineticoDB()->userProfileDAO()->create(*user);

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

		while (UI::UIProcess()) {
			step();
		}

		cleanUp();
		return 0;
	}
	
	void Cinetico::setLanguage(Dictionary::LanguageID langId) {
		m_dictionary->setLanguage(langId);

		//update view
		m_cineticoUI->update();
	}
}

#include <windows.h>
using namespace cinetico;

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Cinetico cinetico;
	return cinetico.run();
}