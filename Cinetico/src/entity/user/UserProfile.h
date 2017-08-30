#ifndef __USERPROFILE_H__
#define __USERPROFILE_H__

#include <string>

class UserProfile
{
	std::string m_username;
	std::string m_password;
	unsigned int m_creationDate;
	bool m_logged;

	bool validatePassword(const std::string &currentPassword);

public:
	UserProfile(const std::string &username, const std::string &password);
	UserProfile(const std::string &username, const std::string &password, unsigned int creationDate);
	bool changePassword(const std::string &oldPassword, const std::string &newPassword);
	bool login(const std::string &password);

	const char *username() const { return m_username.c_str(); }
	const char *password() const { return m_password.c_str(); }
	inline unsigned int creationDate() const { return m_creationDate; }
};

#endif