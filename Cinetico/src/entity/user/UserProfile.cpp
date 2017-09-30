
#include "UserProfile.h"

namespace cinetico {

	bool UserProfile::validatePassword(const std::string &password) {
		return password == m_password;
	}

	UserProfile::UserProfile(const std::string &loginName, const std::string &password, int id) {
		m_id = id;
		m_username = loginName;
		m_password = password;
		m_creationDate = time_t(NULL);
		m_loggedIn = false;
	}

	UserProfile::UserProfile(const std::string &loginName, const std::string &password, unsigned int creationDate, int id) {
		m_id = id;
		m_username = loginName;
		m_password = password;
		m_creationDate = creationDate;
		m_loggedIn = false;
	}

	bool UserProfile::changePassword(const std::string &oldPassword, const std::string &newPassword) {
		if(validatePassword(oldPassword)) {
			m_password = newPassword;
			return true;
		}

		return false;
	}

	bool UserProfile::login(const std::string &password) {
		if(validatePassword(password)) {
			m_loggedIn = true;
		}

		return m_loggedIn;
	}

	bool UserProfile::logoff() {
		if(m_loggedIn) {
			m_loggedIn = false;
			return true;
		}
		return false;
	}

}