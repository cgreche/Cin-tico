#ifndef __CINETICO_USERPROFILE_H__
#define __CINETICO_USERPROFILE_H__

#include <string>

namespace cinetico {

	class UserProfile
	{
		int m_id;
		std::string m_username;
		std::string m_password;
		std::string m_name;
		unsigned int m_creationDate;
		bool m_loggedIn;

		bool validatePassword(const std::string &currentPassword);

	public:
		UserProfile(const std::string &username, const std::string &password, int id = -1);
		UserProfile(const std::string &username, const std::string &password, unsigned int creationDate, int id = -1);
		bool changePassword(const std::string &oldPassword, const std::string &newPassword);
		bool login(const std::string &password);
		bool logoff();

		void setName(const std::string &name) { m_name = name; }

		const int id() const { return m_id; }
		const std::string& username() const { return m_username; }
		const std::string& password() const { return m_password; }
		const std::string& name() const { return m_name; }
		inline unsigned int creationDate() const { return m_creationDate; }
		bool loggedIn() const { return m_loggedIn; }
	};

}

#endif