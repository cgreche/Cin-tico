
#ifndef __LOGIN_CONTROLLER_H__
#define __LOGIN_CONTROLLER_H__

#include "uilib/ui/uibase.h"

class LoginController
{
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
					ComboBox comboUsername;
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
	bool login(const char *username, const char *password);
	void createAccount(const char *username, const char *password);
};

#endif