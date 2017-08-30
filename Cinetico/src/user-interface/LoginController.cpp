
#include "LoginController.h"
#include "entity/user/dao/UserProfileDAO.h"

#include "uilib/ui/uibase.h"

extern UserProfileDAO *userProfileDAO;

Layout *LoginController::viewDefinition() {
	
	labelTitle.setText("Cinético");
	
	labelWelcome.setText("Bem-vindo!");
	labelStartInfo.setText("Para começar, selecione ou crie um Perfil de usuário");
	
	labelLoginTitle.setText("Selecionar Perfil de usuário");
	labelUsername.setText("Nome do usuário");
	labelPassword.setText("Senha");
	buttonLogin.setText("Entrar");

	labelCreateAccountTitle.setText("Criar Perfil de usuário");
	labelCreateAccountUsername.setText("Nome do usuário");
	labelCreateAccountPassword.setText("Senha");
	buttonCreateAccount.setText("Criar conta");

	labelAuthor.setText("2017 César Reche");

	layoutLogin.append(labelLoginTitle);
	layoutLogin.append(labelUsername);
	layoutLogin.append(comboUsername);
	layoutLogin.append(labelPassword);
	layoutLogin.append(editPassword);
	layoutLogin.append(buttonLogin);

	layoutCreateAccount.append(labelCreateAccountTitle);
	layoutCreateAccount.append(labelCreateAccountUsername);
	layoutCreateAccount.append(editCreateAccountUsername);
	layoutCreateAccount.append(labelCreateAccountPassword);
	layoutCreateAccount.append(editCreateAccountPassword);
	layoutCreateAccount.append(buttonCreateAccount);

	layoutStartText.append(labelWelcome);
	layoutStartText.append(labelStartInfo);

	layoutLoginArea.append(layoutLogin);
	layoutLoginArea.append(layoutCreateAccount);
	
	header.append(labelTitle);
	content.append(layoutStartText);
	content.append(layoutLoginArea);
	footer.append(labelAuthor);

	layout.append(header);
	layout.append(content);
	layout.append(footer);

	return &layout;
}

bool LoginController::login(const char *username, const char *password) {
	
	UserProfile *user = userProfileDAO->getByLoginName(username);
	if(user) {
		if(user->login(password)) {
			//redirect to...
			return true;
		}

		delete user;
	}
	return false;
}

void LoginController::createAccount(const char *username, const char *password) {
	UserProfile *user = userProfileDAO->getByLoginName(username);
	if(user) {
		//usuario já existe
		delete user;
		return;
	}

	//Account creation
	user = new UserProfile(username, password);
	userProfileDAO->save(*user);

	//account created successfully
}