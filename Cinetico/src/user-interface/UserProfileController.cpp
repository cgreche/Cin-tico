
#include "cinetico.h"
#include "cineticodb.h"
#include "cineticoui.h"
#include "UserProfileController.h"
#include "utils/crypter.h"
#include <time.h>

namespace cinetico {

	static void buttonBack_onClick(Button& button) {
		UserProfileController *controller = (UserProfileController*)button.param();
		controller->m_cineticoUI.goTo(CineticoUI::EXERCISES);
	}

	static void buttonChangeUserDetails_onClick(Button& button) {
		UserProfileController *controller = (UserProfileController*)button.param();
		controller->onChangeUserDetails();
	}

	static void buttonChangePassword_onClick(Button& button) {
		UserProfileController *controller = (UserProfileController*)button.param();
		controller->onChangePassword();
	}

	static void buttonDeactivateUser_onClick(Button &button) {
		UserProfileController *controller = (UserProfileController*)button.param();
		
		Message::message_result result = Message::question(NULL, controller->m_dictionary.getString(Dictionary::UserProfileViewQuestionDeactivateConfirmation));
		if (result == Message::yes) {
			controller->m_cinetico.logoffCurrentUser();
			controller->m_cineticoUI.goTo(CineticoUI::LOGIN);
			controller->m_cinetico.cineticoDB()->userProfileDAO()->exclude(*controller->m_currentUser);
		}
	}

	void UserProfileController::onViewUpdate() {
		title.setTitle(m_dictionary.getString(Dictionary::UserProfileViewTitle));
		title.setDesc(m_dictionary.getString(Dictionary::UserProfileViewDesc));
		buttonBack.setText(m_dictionary.getString(Dictionary::DefaultActionBack));

		separatorUserDetails.setText(m_dictionary.getString(Dictionary::UserProfileViewSectionUserDetails));
		labelCreationDateDescr.setText(m_dictionary.getString(Dictionary::UserProfileCreationDate));

		labelUserDescr.setText(m_dictionary.getString(Dictionary::UserProfileLoginName));

		tbUsername.setLabel(m_dictionary.getString(Dictionary::UserProfileUserName));
		buttonChangeUserDetails.setText(m_dictionary.getString(Dictionary::DefaultActionEdit));

		separatorPassword.setText(m_dictionary.getString(Dictionary::UserProfileViewSectionChangePassword));
		tbOldPassword.setLabel(m_dictionary.getString(Dictionary::UserProfileViewCurrentPassword));
		tbNewPassword.setLabel(m_dictionary.getString(Dictionary::UserProfileViewNewPassword));
		tbNewPasswordConfirmation.setLabel(m_dictionary.getString(Dictionary::UserProfileViewNewPasswordConfirmation));
		buttonChangePassword.setText(m_dictionary.getString(Dictionary::DefaultActionEdit));

		separatorDeactivateUser.setText(m_dictionary.getString(Dictionary::UserProfileViewSectionDeactivateUser));
		buttonDeactivateUser.setText(m_dictionary.getString(Dictionary::UserProfileViewDeactivateUser));
	}

	UserProfileController::UserProfileController(CineticoUI &cineticoUI)
		: Controller(cineticoUI)
	{
		
		buttonBack.setParam(this);
		buttonBack.setOnClick(buttonBack_onClick);
		layoutActions.append(buttonBack);

		layoutCreationDate.append(labelCreationDateDescr);
		layoutCreationDate.append(labelCreationDate);
		
		layoutUser.append(labelUserDescr);
		layoutUser.append(labelUser);

		buttonChangeUserDetails.setParam(this);
		buttonChangeUserDetails.setOnClick(buttonChangeUserDetails_onClick);

		layoutUserDetailsSection.append(separatorUserDetails);
		layoutUserDetailsSection.append(layoutCreationDate);
		layoutUserDetailsSection.append(layoutUser);
		layoutUserDetailsSection.append(tbUsername);
		layoutUserDetailsSection.append(buttonChangeUserDetails);

		buttonChangePassword.setParam(this);
		buttonChangePassword.setOnClick(buttonChangePassword_onClick);

		layoutPassword.append(tbOldPassword);
		layoutPassword.append(tbNewPassword);
		layoutPassword.append(tbNewPasswordConfirmation);
		
		layoutPasswordSection.append(separatorPassword);
		layoutPasswordSection.append(layoutPassword);
		layoutPasswordSection.append(buttonChangePassword);

		buttonDeactivateUser.setParam(this);
		buttonDeactivateUser.setOnClick(buttonDeactivateUser_onClick);
		layoutDeactivateUser.append(separatorDeactivateUser);
		layoutDeactivateUser.append(buttonDeactivateUser);

		layout.append(title);
		layout.append(layoutActions);
		layout.append(separatorActions);
		layout.append(layoutUserDetailsSection);
		layout.append(layoutPasswordSection);
		layout.append(layoutDeactivateUser);
		layout.setMargin(10);
	}

	Layout *UserProfileController::viewDefinition() {
		return &layout;
	}

	void UserProfileController::onViewEnter(ViewParams *params) {
		m_currentUser = (UserProfile*)(*params)["user"];

		char datetime[256];
		time_t now = ::time(0);
		tm *ltm = ::localtime(&now);		

		sprintf(datetime,"%02d/%02d/%02d %02d:%02d:%02d",ltm->tm_mday,ltm->tm_mon+1,ltm->tm_year+1900,ltm->tm_hour,ltm->tm_min,ltm->tm_sec);
		labelCreationDate.setText(datetime);
		labelUser.setText(m_currentUser->loginName().c_str());
		layout.setSize(layout.size());

		tbUsername.setText(m_currentUser->name().c_str());
		tbOldPassword.setText("");
		tbNewPassword.setText("");
		tbNewPasswordConfirmation.setText("");
	}

	void UserProfileController::onViewTick() {
	}

	void UserProfileController::onViewQuit() {

	}

	void UserProfileController::onChangeUserDetails() {
		string name = tbUsername.text();
		m_currentUser->setName(name.data());
		m_cinetico.cineticoDB()->userProfileDAO()->update(*m_currentUser);
		Message::msg(NULL, m_dictionary.getString(Dictionary::UserProfileViewErrorUserNameChangedSucessfully));
	}

	void UserProfileController::onChangePassword() {
		string curPw = tbOldPassword.text();
		string newPw = tbNewPassword.text();
		string newPwConfirm = tbNewPasswordConfirmation.text();

		if (newPw != newPwConfirm) {
			Message::error(NULL, m_dictionary.getString(Dictionary::UserProfileViewErrorPasswordConfirmationNotEqual));
			return;
		}

		std::string cryptCurPw = Crypter::SimpleHash(curPw.data());
		std::string crypTNewPw = Crypter::SimpleHash(newPw.data());
		if(!m_currentUser->changePassword(cryptCurPw, crypTNewPw)) {
			Message::error(NULL, m_dictionary.getString(Dictionary::UserProfileViewErrorPasswordWrongCurrentPassword));
			return;
		}

		m_cinetico.cineticoDB()->userProfileDAO()->update(*m_currentUser);
		Message::msg(NULL,m_dictionary.getString(Dictionary::UserProfileViewErrorPasswordChangedSucessfully));

		tbOldPassword.setText("");
		tbNewPassword.setText("");
		tbNewPasswordConfirmation.setText("");
	}

}