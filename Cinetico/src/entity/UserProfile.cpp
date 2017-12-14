
#include "UserProfile.h"
#include <time.h>

namespace cinetico {

	bool UserProfile::validatePassword(const std::string &password) {
		return password == m_password;
	}

	UserProfile::UserProfile(const std::string &loginName, const std::string &password, const std::string &name, int id) {
		m_id = id;
		m_loginName = loginName;
		m_password = password;
		m_name = name;
		m_creationDate = (unsigned int)::time(NULL);
		m_loggedIn = false;
	}

	UserProfile::UserProfile(const std::string &loginName, const std::string &password, const std::string &name, unsigned int creationDate, int id) {
		m_id = id;
		m_loginName = loginName;
		m_password = password;
		m_name = name;
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