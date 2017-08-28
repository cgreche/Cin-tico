#ifndef __USERPROFILE_H__
#define __USERPROFILE_H__

class UserProfile
{
	std::string m_loginName;
	std::string m_password;
	int m_creationDate;
	bool m_logged;


	bool validatePassword(const std::string &currentPassword);

public:
	static UserProfile create(const std::string &loginName, const std::string &password);
	static UserProfile *search(const std::string &loginName);
	static UserProfile *getByLoginName(const td::string &loginName);
	void changePassword(const std::string &oldPassword, const std::string &newPassword);
	void deleteSelf();
	void login(const std::string &password);
};



#endif