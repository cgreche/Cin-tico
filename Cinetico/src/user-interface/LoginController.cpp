
#include "cinetico.h"
#include "cineticodb.h"
#include "cineticoui.h"
#include "LoginController.h"
#include "entity/dao/UserProfileDAO.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	void onClick_login(Button &button) {
		LoginController *controller = (LoginController*)button.param();
		controller->login();
	}

	void onClick_createAccount(Button &button) {
		LoginController *controller = (LoginController*)button.param();
		controller->createAccount();
	}

	void LoginController::onViewUpdate() {
		title.setTitle(m_dictionary.getString(Dictionary::LoginViewTitle));
		title.setDesc(m_dictionary.getString(Dictionary::LoginViewDesc));
		separatorLogin.setText(m_dictionary.getString(Dictionary::LoginViewSectionLogin));
		tbLoginUsername.setLabel(m_dictionary.getString(Dictionary::LoginViewUsername) + '*');
		tbLoginPassword.setLabel(m_dictionary.getString(Dictionary::LoginViewPassword));
		
		buttonLogin.setText(m_dictionary.getString(Dictionary::LoginViewActionLogin));
		separatorCreateAccount.setText(m_dictionary.getString(Dictionary::LoginViewSectionCreateUserProfile));
		tbCreateAccountUsername.setLabel(m_dictionary.getString(Dictionary::LoginViewUsername) + '*');
		tbCreateAccountPassword.setLabel(m_dictionary.getString(Dictionary::LoginViewPassword));

		buttonCreateAccount.setText(m_dictionary.getString(Dictionary::LoginViewActionCreateUserProfile));
	}

	LoginController::LoginController(CineticoUI &cineticoUI)
		: Controller(cineticoUI)
	{
		tbLoginPassword.setPasswordMode(true);
		buttonLogin.setParam(this);
		buttonLogin.setOnClick(onClick_login);

		tbCreateAccountPassword.setPasswordMode(true);
		buttonCreateAccount.setParam(this);
		buttonCreateAccount.setOnClick(onClick_createAccount);

		layoutLogin.append(separatorLogin);
		layoutLogin.append(tbLoginUsername);
		layoutLogin.append(tbLoginPassword);
		layoutLogin.append(buttonLogin, Size(SizeTypeMax, SizeTypeAuto));

		layoutCreateAccount.append(separatorCreateAccount);
		layoutCreateAccount.append(tbCreateAccountUsername);
		layoutCreateAccount.append(tbCreateAccountPassword);
		layoutCreateAccount.append(buttonCreateAccount, Size(SizeTypeMax, SizeTypeAuto));

		layoutLoginArea.append(layoutLogin, AutoSize, 40);
		layoutLoginArea.append(layoutCreateAccount);

		layout.append(title);
		layout.append(layoutLoginArea);
		layout.setMargin(10);

		
		testView.setBackgroundColor(uilib::Color(200, 200, 200));
		testButton.setText("TESTTTTT");
		testLayout.append(testButton,Size(SizeTypeMax,SizeTypeAuto));
		testLayout.append(testButton2);
		testLayout.append(testButton3);
		testLayout.append(testButton4);
		testView.setLayout(&testLayout);
		layout.append(testView,Size(300,100));
	}

	Layout *LoginController::viewDefinition() {
		return &layout;
	}

	void LoginController::onViewEnter(ViewParams params) {
		m_userProfileDAO = m_cinetico.cineticoDB()->userProfileDAO();
	}

	void LoginController::onViewQuit() {

	}

	void LoginController::login() {
		string &username = tbLoginUsername.text();
		string &password = tbLoginPassword.text();

		Cinetico::CineticoError res = m_cinetico.loginUser(username.data(), password.data());
		if (res == Cinetico::SUCCESS) {
			string str = m_dictionary.getString(Dictionary::LoginViewErrorUserLoggedSucessfully, username);
			Message::msg(NULL, str);
			m_cineticoUI.goTo(CineticoUI::EXERCISES);
			//
			tbLoginUsername.setText("");
			tbLoginPassword.setText("");
		}
		else {
			Message::error(NULL, m_dictionary.getString(Dictionary::LoginViewErrorInvalidCredentials, username));
		}
	}

	void LoginController::createAccount() {
		string &username = tbCreateAccountUsername.text();
		string &password = tbCreateAccountPassword.text();
		
		//Validação de preenchimento
		if (username == "")
			return;

		bool blank = true;
		for (unsigned int i = 0; i < username.length(); ++i) {
			if (!isspace(username.data()[i])) {
				blank = false;
				break;
			}
		}

		if (blank)
			return;
		//

		Cinetico::CineticoError res = m_cinetico.createAccount(username.data(), password.data());
		if (res == Cinetico::SUCCESS) {
			Message::msg(NULL, m_cinetico.dictionary()->getString(Dictionary::LoginViewErrorUserProfileCreatedSuccessfully, username.data()));

			tbCreateAccountUsername.setText("");
			tbCreateAccountPassword.setText("");
			return;
		}
		else if(res == Cinetico::USER_ALREADY_EXISTS) {
			Message::warning(NULL, m_cinetico.dictionary()->getString(Dictionary::LoginViewErrorUsernameAlreadyExists, username.data()));
			return;
		}
	}

}