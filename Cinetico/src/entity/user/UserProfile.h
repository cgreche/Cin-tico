#ifndef __CINETICO_USERPROFILE_H__
#define __CINETICO_USERPROFILE_H__

#include <string>

namespace cinetico {

	class UserProfile
	{
		int m_id;
		std::string m_loginName;
		std::string m_password;
		std::string m_name;
		unsigned int m_creationDate;
		bool m_loggedIn;

	public:
		UserProfile(const std::string &loginName, const std::string &password, const std::string &name, int id = -1);
		UserProfile(const std::string &loginName, const std::string &password, const std::string &name, unsigned int creationDate, int id = -1);
		bool validatePassword(const std::string &currentPassword);
		bool changePassword(const std::string &oldPassword, const std::string &newPassword);
		bool login(const std::string &password);
		bool logoff();

		inline void setName(const std::string &name) { m_name = name; }

		inline const int id() const { return m_id; }
		inline const std::string& loginName() const { return m_loginName; }
		inline const std::string& password() const { return m_password; }
		inline const std::string& name() const { return m_name; }
		inline unsigned int creationDate() const { return m_creationDate; }
		inline bool loggedIn() const { return m_loggedIn; }
	};

}

#endif