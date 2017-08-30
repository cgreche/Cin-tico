#ifndef __USERPROFILEDAO_H__
#define __USERPROFILEDAO_H__

#include <vector>
#include <string>

#include "entity/user/UserProfile.h"

class Database;

class UserProfileDAO
{
	Database &m_db;

public:
	UserProfileDAO(Database &db);

	void save(UserProfile &profile);
	void update(UserProfile &user);
	std::vector<UserProfile *> getAll();
	UserProfile *getByLoginName(const char *username);
	void exclude(UserProfile &user);
	void exclude(const char *username);
};



#endif