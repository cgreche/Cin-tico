
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
		: title("Bem-vindo!", "Para come�ar, selecione ou crie um Perfil de usu�rio")
	{

		separatorLogin.setText("Entrar com Perfil de usu�rio");
		tbLoginUsername.setLabel("Nome do usu�rio *");
		tbLoginPassword.setLabel("Senha *");
		tbLoginPassword.setPasswordMode(true);
		buttonLogin.setText("Entrar");
		buttonLogin.setParam(this);
		buttonLogin.setOnClick(onClick_login);

		separatorCreateAccount.setText("Criar Perfil de usu�rio");
		tbCreateAccountUsername.setLabel("Nome do usu�rio *");
		tbCreateAccountPassword.setLabel("Senha *");
		tbCreateAccountPassword.setPasswordMode(true);

		buttonCreateAccount.setText("Criar conta");
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
			string str = "Sucesso! Usu�rio logado como ";
			str += username;
			str += ".";
			Message::msg(NULL, str);
			g_cinetico.goTo(Cinetico::EXERCISES);
			//
			tbLoginUsername.setText("");
			tbLoginPassword.setText("");
		}
		else {
			Message::error(NULL, "Usu�rio ou senha inv�lidos.");
		}
	}

	void LoginController::createAccount() {
		string &username = tbCreateAccountUsername.text();
		string &password = tbCreateAccountPassword.text();
		
		//Valida��o de preenchimento
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
			Message::msg(NULL, "Conta criada com sucesso.");

			tbCreateAccountUsername.setText("");
			tbCreateAccountPassword.setText("");
			return;
		}
		else if(res == Cinetico::USER_ALREADY_EXISTS) {
			Message::warning(NULL, "Nome de usu�rio j� existente.");
			return;
		}
	}

}