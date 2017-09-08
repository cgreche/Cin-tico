
#include "LoginController.h"
#include "entity/user/dao/UserProfileDAO.h"
#include "utils/crypter.h"
#include "uilib/ui/uibase.h"

extern UserProfileDAO *userProfileDAO;

void onClick_login(Button &button) {
	LoginController *controller = (LoginController*)button.param();
	controller->login();
}

void onClick_createAccount(Button &button) {
	LoginController *controller = (LoginController*)button.param();
	controller->createAccount();
}

Layout *LoginController::viewDefinition() {

	labelTitle.setText("Cinético");
	labelTitle.setTextColor(Color(0x80, 0x80, 0x80));
	labelTitle.setAlignment(Label::VCenter);
	labelTitle.setBackgroundColor(Color(0, 0, 0));
	labelTitle.setTransparent(false);
	labelTitle.setFont(FontDesc("Arial",16,FONT_BOLD));

	labelWelcome.setText("Bem-vindo!");
	labelWelcome.setFont(FontDesc("Arial", 46, FONT_BOLD));
	labelStartInfo.setText("Para começar, selecione ou crie um Perfil de usuário");
	labelStartInfo.setFont(FontDesc("Arial", 10, FONT_BOLD));

	labelLoginTitle.setText("Selecionar Perfil de usuário");
	labelLoginTitle.setFont(FontDesc("Arial", 18, FONT_BOLD));
	labelUsername.setText("Nome do usuário");
	labelPassword.setText("Senha");
	editPassword.setPasswordMode(true);
	buttonLogin.setText("Entrar");
	buttonLogin.setParam(this);
	buttonLogin.setOnClick(onClick_login);

	labelCreateAccountTitle.setText("Criar Perfil de usuário");
	labelCreateAccountTitle.setFont(FontDesc("Arial", 20, FONT_BOLD));
	labelCreateAccountUsername.setText("Nome do usuário");
	labelCreateAccountPassword.setText("Senha");
	editCreateAccountPassword.setPasswordMode(true);
	buttonCreateAccount.setText("Criar conta");
	buttonCreateAccount.setParam(this);
	buttonCreateAccount.setOnClick(onClick_createAccount);

	labelAuthor.setText("2017 César Reche");
	labelAuthor.setTransparent(false);

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
	layoutCreateAccount.append(buttonCreateAccount,Size(SizeTypeMax,SizeTypeAuto));

	layoutStartText.append(labelWelcome);
	layoutStartText.append(labelStartInfo);

	layoutLoginArea.append(layoutLogin,AutoSize,40);
	layoutLoginArea.append(layoutCreateAccount);

	header.append(labelTitle,Size(SizeTypeMax,60));
	content.append(layoutStartText,AutoSize,20);
	content.append(layoutLoginArea);
	footer.append(labelAuthor, MaximumSize);

	layout.append(header, AutoSize, 20);
	layout.append(content, AutoSize, 20);
	layout.append(footer);

	layout.setMargin(10);
	return &layout;
}

void LoginController::onViewEnter() {
}

void LoginController::onViewQuit() {

}

void LoginController::login() {
	string &username = editUsername.text();
	string &password = editPassword.text();
	std::string cryptPW = Crypter::SimpleHash(password.data());
	UserProfile *user = userProfileDAO->getByLoginName(username.data());
	if (user) {
		if (user->login(cryptPW)) {
			//redirect to... (to-do)
			//Temporary code
			string str = "Sucesso! Usuário logado como ";
			str += username;
			str += ". Get into the 3D World.";
			Message::msg(NULL, str);
			this->layout.parentControl()->setVisible(false);
			extern void setupWorld3D();
			setupWorld3D();
			extern bool g_onWorld3D;
			g_onWorld3D = true;
			return;
		}
		delete user;
	}

	Message::error(NULL, "Usuário ou senha inválida.");
}

void LoginController::createAccount() {
	string &username = editCreateAccountUsername.text();
	string &password = editCreateAccountPassword.text();
	std::string cryptPW = Crypter::SimpleHash(password.data());

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

	if(blank)
		return;
	//

	UserProfile *user = userProfileDAO->getByLoginName(username.data());
	if(user) {
		//usuario já existe
		Message::warning(NULL, "Nome de usuário já existente.");
		delete user;
		return;
	}

	//Account creation
	user = new UserProfile(username.data(), cryptPW);
	userProfileDAO->save(*user);

	//account created successfully
	Message::msg(NULL,"Conta criada com sucesso.");

	editCreateAccountUsername.setText("");
	editCreateAccountPassword.setText("");
}