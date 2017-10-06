
#ifndef __CINETICO_LOGINCONTROLLER_H__
#define __CINETICO_LOGINCONTROLLER_H__

#include "Controller.h"
#include "components/PageTitle.h"
#include "components/TextBox.h"

namespace cinetico {
	using namespace uilib;

	class UserProfileDAO;

	class LoginController : public Controller
	{
	public:
		VerticalLayout layout;
			PageTitle title;
			HorizontalLayout layoutLoginArea;
				VerticalLayout layoutLogin;
					Label labelLoginTitle;
					TextBox tbLoginUsername;
					TextBox tbLoginPassword;
					Button buttonLogin;
				VerticalLayout layoutCreateAccount;
					Label labelCreateAccountTitle;
					TextBox tbCreateAccountUsername;
					TextBox tbCreateAccountPassword;
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