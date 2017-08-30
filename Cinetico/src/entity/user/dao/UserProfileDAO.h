#ifndef __USERPROFILEDAO_H__
#define __USERPROFILEDAO_H__

#include <vector>
#include <string>

#include "entity/user/UserProfile.h"

class UserProfileDAO
{
	Database &m_db;

public:
	UserProfileDAO(Database &db);

	UserProfile *create(const char *loginName, const char *password);
	void update(UserProfile &user);
	std::vector<UserProfile *> getAll();
	UserProfile *getByLoginName(const char *loginName);
	void exclude(UserProfile &user);
	void exclude(const char *loginName);
};



#endif