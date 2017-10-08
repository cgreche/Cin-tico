
#ifndef __CINETICO_LOGINCONTROLLER_H__
#define __CINETICO_LOGINCONTROLLER_H__

#include "Controller.h"
#include "components/cPageTitle.h"
#include "components/cTextBox.h"

namespace cinetico {
	using namespace uilib;

	class UserProfileDAO;

	class LoginController : public Controller
	{
	public:
		VerticalLayout layout;
			cPageTitle title;
			HorizontalLayout layoutLoginArea;
				VerticalLayout layoutLogin;
					Label labelLoginTitle;
					cTextBox tbLoginUsername;
					cTextBox tbLoginPassword;
					Button buttonLogin;
				VerticalLayout layoutCreateAccount;
					Label labelCreateAccountTitle;
					cTextBox tbCreateAccountUsername;
					cTextBox tbCreateAccountPassword;
					Button buttonCreateAccount;

		UserProfileDAO *m_userProfileDAO;
	public:
		LoginController();

		Layout *viewDefinition();
		void onViewEnter(ViewParams *params);
		void onViewQuit();
		void login();
		void createAccount();
	};

}

#endif