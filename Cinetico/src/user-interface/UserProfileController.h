
#ifndef __CINETICO_USERPROFILECONTROLLER_H__
#define __CINETICO_USERPROFILECONTROLLER_H__

#include "Controller.h"
#include "uilib/ui/uibase.h"
#include "components/cPageTitle.h"
#include "components/cTextBox.h"
#include "components/cSeparator.h"

namespace cinetico {

	using namespace uilib;
	class UserProfile;

	class UserProfileController : public Controller
	{
	public:
		VerticalLayout layout;
			cPageTitle title;
			HorizontalLayout layoutActions;
				Button buttonBack;
			Separator separatorActions;
			VerticalLayout layoutUserDetailsSection;
				cSeparator separatorUserDetails;
				HorizontalLayout layoutCreationDate;
					Label labelCreationDateDescr;
					Label labelCreationDate;
					HorizontalLayout layoutUser;
					Label labelUserDescr;
					Label labelUser;
				cTextBox tbUsername;
				Button buttonChangeUserDetails;

			VerticalLayout layoutPasswordSection;
				cSeparator separatorPassword;
				VerticalLayout layoutPassword;
					cTextBox tbOldPassword;
					cTextBox tbNewPassword;
					cTextBox tbNewPasswordConfirmation;
				Button buttonChangePassword;

			//

		UserProfile *m_currentUser;

		void onChangeUserDetails();
		void onChangePassword();

		friend void buttonBack_onClick(Button& button);
		friend void buttonChangeUserDetails_onClick(Button& button);
		friend void buttonChangePassword_onClick(Button& button);
	public:
		UserProfileController();

		Layout *viewDefinition();
		void onViewEnter(ViewParams *params);
		void onViewTick();
		void onViewQuit();
	};

}

#endif