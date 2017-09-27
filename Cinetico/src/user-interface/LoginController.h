
#ifndef __CINETICO_LOGINCONTROLLER_H__
#define __CINETICO_LOGINCONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	class UserProfileDAO;

	class LoginController : public Controller
	{
	public:
		VerticalLayout layout;
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


		UserProfileDAO *m_userProfileDAO;
	public:
		LoginController();

		Layout *viewDefinition();
		void onViewEnter();
		void onViewQuit();
		void login();
		void createAccount();
	};

}

#endif