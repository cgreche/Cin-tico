
#ifndef __LOGIN_CONTROLLER_H__
#define __LOGIN_CONTROLLER_H__

#include "uilib/ui/uibase.h"

class LoginController
{
public:
	VerticalLayout layout;
		VerticalLayout header;
			Label labelTitle;
		VerticalLayout content;
			VerticalLayout layoutStartText;
				Label labelWelcome;
				Label labelStartInfo;
			HorizontalLayout layoutLoginArea;
				VerticalLayout layoutLogin;
					Label labelLoginTitle;
					Label labelUsername;
					EditBox editUsername;
					Label labelPassword;
					EditBox editPassword;
					Button buttonLogin;
				VerticalLayout layoutCreateAccount;
					Label labelCreateAccountTitle;
					Label labelCreateAccountUsername;
					EditBox editCreateAccountUsername;
					Label labelCreateAccountPassword;
					EditBox editCreateAccountPassword;
					Button buttonCreateAccount;
		VerticalLayout footer;
			Label labelAuthor;
public:
	Layout *viewDefinition();
	void onViewEnter();
	void onViewQuit();
	void login();
	void createAccount();
};

#endif