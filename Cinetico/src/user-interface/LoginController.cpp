
#include "LoginController.h"
#include "entity/user/dao/UserProfileDAO.h"
#include "utils/crypter.h"
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

	LoginController::LoginController() {
		labelWelcome.setText("Bem-vindo!");
		labelWelcome.setFont(FontDesc("Arial", 46, FONT_BOLD));
		labelStartInfo.setText("Para come�ar, selecione ou crie um Perfil de usu�rio");
		labelStartInfo.setFont(FontDesc("Arial", 10, FONT_BOLD));

		labelLoginTitle.setText("Selecionar Perfil de usu�rio");
		labelLoginTitle.setFont(FontDesc("Arial", 18, FONT_BOLD));
		labelUsername.setText("Nome do usu�rio");
		labelPassword.setText("Senha");
		editPassword.setPasswordMode(true);
		buttonLogin.setText("Entrar");
		buttonLogin.setParam(this);
		buttonLogin.setOnClick(onClick_login);

		labelCreateAccountTitle.setText("Criar Perfil de usu�rio");
		labelCreateAccountTitle.setFont(FontDesc("Arial", 18, FONT_BOLD));
		labelCreateAccountUsername.setText("Nome do usu�rio");
		labelCreateAccountPassword.setText("Senha");
		editCreateAccountPassword.setPasswordMode(true);
		buttonCreateAccount.setText("Criar conta");
		buttonCreateAccount.setParam(this);
		buttonCreateAccount.setOnClick(onClick_createAccount);

		layoutLogin.append(labelLoginTitle);
		layoutLogin.append(labelUsername);
		layoutLogin.append(editUsername);
		layoutLogin.append(labelPassword);
		layoutLogin.append(editPassword);
		layoutLogin.append(buttonLogin, Size(SizeTypeMax, SizeTypeAuto));

		layoutCreateAccount.append(labelCreateAccountTitle);
		layoutCreateAccount.append(labelCreateAccountUsername);
		layoutCreateAccount.append(editCreateAccountUsername);
		layoutCreateAccount.append(labelCreateAccountPassword);
		layoutCreateAccount.append(editCreateAccountPassword);
		layoutCreateAccount.append(buttonCreateAccount, Size(SizeTypeMax, SizeTypeAuto));

		layoutStartText.append(labelWelcome);
		layoutStartText.append(labelStartInfo);

		layoutLoginArea.append(layoutLogin, AutoSize, 40);
		layoutLoginArea.append(layoutCreateAccount);

		layout.append(layoutStartText, AutoSize, 20);
		layout.append(layoutLoginArea);

		layout.setMargin(10);
	}

	Layout *LoginController::viewDefinition() {
		return &layout;
	}

	void LoginController::onViewEnter() {
		m_userProfileDAO = g_cinetico.cineticoDB()->userProfileDAO();
	}

	void LoginController::onViewQuit() {

	}

	void LoginController::login() {
		string &username = editUsername.text();
		string &password = editPassword.text();

		//todo: send login validation to Cinetico class
		UserProfile *user = m_userProfileDAO->getByLoginName(username.data());
		if (user) {
			std::string cryptPW = Crypter::SimpleHash(password.data());
			if (user->login(cryptPW)) {
				g_cinetico.setUser(user);
				string str = "Sucesso! Usu�rio logado como ";
				str += username;
				Message::msg(NULL, str);
				g_cinetico.goTo(Cinetico::EXERCISES);
				return;
			}
			delete user;
		}

		Message::error(NULL, "Usu�rio ou senha inv�lida.");
	}

	void LoginController::createAccount() {
		string &username = editCreateAccountUsername.text();
		string &password = editCreateAccountPassword.text();
		std::string cryptPW = Crypter::SimpleHash(password.data());

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

		UserProfile *user = m_userProfileDAO->getByLoginName(username.data());
		if (user) {
			//usuario j� existe
			Message::warning(NULL, "Nome de usu�rio j� existente.");
			delete user;
			return;
		}

		//Account creation
		user = new UserProfile(username.data(), cryptPW);
		m_userProfileDAO->save(*user);

		//account created successfully
		Message::msg(NULL, "Conta criada com sucesso.");

		editCreateAccountUsername.setText("");
		editCreateAccountPassword.setText("");
	}

}