
#include "LoginController.h"
#include "entity/dao/UserProfileDAO.h"
#include "uilib/ui/uibase.h"
#include "cinetico.h"

namespace cinetico {

	extern Cinetico g_cinetico;

	void onClick_login(Button &button) {
		LoginController *controller = (LoginController*)button.param();
		controller->login();
	}

	void onClick_createAccount(Button &button) {
		LoginController *controller = (LoginController*)button.param();
		controller->createAccount();
	}

	LoginController::LoginController()
		: title(g_cinetico.dictionary()->getString(Dictionary::LoginViewTitle).c_str(),
			g_cinetico.dictionary()->getString(Dictionary::LoginViewDesc).c_str())
	{
		Dictionary *dictionary = g_cinetico.dictionary();
		separatorLogin.setText(dictionary->getString(Dictionary::LoginViewSectionLogin).c_str());
		tbLoginUsername.setLabel(dictionary->getString(Dictionary::LoginViewUsername).c_str());
		tbLoginPassword.setLabel(dictionary->getString(Dictionary::LoginViewPassword).c_str());
		tbLoginPassword.setPasswordMode(true);
		buttonLogin.setText(dictionary->getString(Dictionary::LoginViewActionLogin).c_str());
		buttonLogin.setParam(this);
		buttonLogin.setOnClick(onClick_login);

		separatorCreateAccount.setText(dictionary->getString(Dictionary::LoginViewSectionCreateUserProfile).c_str());
		tbCreateAccountUsername.setLabel(dictionary->getString(Dictionary::LoginViewUsername).c_str());
		tbCreateAccountPassword.setLabel(dictionary->getString(Dictionary::LoginViewPassword).c_str());
		tbCreateAccountPassword.setPasswordMode(true);

		buttonCreateAccount.setText(dictionary->getString(Dictionary::LoginViewActionCreateUserProfile).c_str());
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
	}

	Layout *LoginController::viewDefinition() {
		return &layout;
	}

	void LoginController::onViewEnter(ViewParams *params) {
		m_userProfileDAO = g_cinetico.cineticoDB()->userProfileDAO();
	}

	void LoginController::onViewQuit() {

	}

	void LoginController::login() {
		string &username = tbLoginUsername.text();
		string &password = tbLoginPassword.text();

		Cinetico::CineticoError res = g_cinetico.loginUser(username.data(), password.data());
		if (res == Cinetico::SUCCESS) {
			string str = g_cinetico.dictionary()->getString(Dictionary::LoginViewErrorUserLoggedSucessfully, username).c_str();
			Message::msg(NULL, str);
			g_cinetico.goTo(Cinetico::EXERCISES);
			//
			tbLoginUsername.setText("");
			tbLoginPassword.setText("");
		}
		else {
			Message::error(NULL, g_cinetico.dictionary()->getString(Dictionary::LoginViewErrorInvalidCredentials, username).c_str());
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

		Cinetico::CineticoError res = g_cinetico.createAccount(username.data(), password.data());
		if (res == Cinetico::SUCCESS) {
			Message::msg(NULL, g_cinetico.dictionary()->getString(Dictionary::LoginViewErrorUserProfileCreatedSuccessfully, username).c_str());

			tbCreateAccountUsername.setText("");
			tbCreateAccountPassword.setText("");
			return;
		}
		else if(res == Cinetico::USER_ALREADY_EXISTS) {
			Message::warning(NULL, g_cinetico.dictionary()->getString(Dictionary::LoginViewErrorUsernameAlreadyExists, username).c_str());
			return;
		}
	}

}