
#include "UserProfile.h"

bool UserProfile::validatePassword(const std::string &password) {
	return password == m_password;
}

UserProfile::UserProfile(const std::string &loginName, const std::string &password) {
	m_username = loginName;
	m_password = password;
	m_creationDate = time_t(NULL);
	m_logged = false;
}

UserProfile::UserProfile(const std::string &loginName, const std::string &password, unsigned int creationDate) {
	m_username = loginName;
	m_password = password;
	m_creationDate = creationDate;
	m_logged = false;
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
		m_logged = true;
	}

	return m_logged;
}