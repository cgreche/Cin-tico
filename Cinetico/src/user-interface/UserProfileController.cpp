
#include "cinetico.h"
#include "UserProfileController.h"
#include "utils/crypter.h"
#include <time.h>

namespace cinetico {
	extern Cinetico g_cinetico;

	static void buttonBack_onClick(Button& button) {
		UserProfileController *controller = (UserProfileController*)button.param();
		g_cinetico.goTo(Cinetico::EXERCISES);
	}

	static void buttonChangeUserDetails_onClick(Button& button) {
		UserProfileController *controller = (UserProfileController*)button.param();
		controller->onChangeUserDetails();
	}

	static void buttonChangePassword_onClick(Button& button) {
		UserProfileController *controller = (UserProfileController*)button.param();
		controller->onChangePassword();
	}

	UserProfileController::UserProfileController()
		: title("Perfil de usu�rio", "Altere informa��es de ou exclua seu perfil de usu�rio.")
	{
		buttonBack.setText("Voltar");
		buttonBack.setParam(this);
		buttonBack.setOnClick(buttonBack_onClick);
		layoutActions.append(buttonBack);

		separatorUserDetails.setText("Detalhes do usu�rio");
		labelCreationDateDescr.setText("Data de cria��o:");
		layoutCreationDate.append(labelCreationDateDescr);
		layoutCreationDate.append(labelCreationDate);
		
		labelUserDescr.setText("Usu�rio:");
		layoutUser.append(labelUserDescr);
		layoutUser.append(labelUser);

		tbUsername.setLabel("Nome do usu�rio");
		buttonChangeUserDetails.setText("Alterar");
		buttonChangeUserDetails.setParam(this);
		buttonChangeUserDetails.setOnClick(buttonChangeUserDetails_onClick);

		layoutUserDetailsSection.append(separatorUserDetails);
		layoutUserDetailsSection.append(layoutCreationDate);
		layoutUserDetailsSection.append(layoutUser);
		layoutUserDetailsSection.append(tbUsername);
		layoutUserDetailsSection.append(buttonChangeUserDetails);

		separatorPassword.setText("Mudar senha");
		tbOldPassword.setLabel("Senha atual");
		tbNewPassword.setLabel("Nova senha");
		tbNewPasswordConfirmation.setLabel("Confirme a nova senha");
		buttonChangePassword.setText("Alterar");
		buttonChangePassword.setParam(this);
		buttonChangePassword.setOnClick(buttonChangePassword_onClick);

		layoutPassword.append(tbOldPassword);
		layoutPassword.append(tbNewPassword);
		layoutPassword.append(tbNewPasswordConfirmation);
		
		layoutPasswordSection.append(separatorPassword);
		layoutPasswordSection.append(layoutPassword);
		layoutPasswordSection.append(buttonChangePassword);

		layout.append(title);
		layout.append(layoutActions);
		layout.append(separatorActions);
		layout.append(layoutUserDetailsSection);
		layout.append(layoutPasswordSection);
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

		tbUsername.setText("");
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
		g_cinetico.cineticoDB()->userProfileDAO()->update(*m_currentUser);
	}

	void UserProfileController::onChangePassword() {
		string curPw = tbOldPassword.text();
		string newPw = tbNewPassword.text();
		string newPwConfirm = tbNewPasswordConfirmation.text();

		if (newPw != newPwConfirm) {
			Message::error(NULL, "Confirma��o de nova senha n�o confere.");
			return;
		}

		std::string cryptCurPw = Crypter::SimpleHash(curPw.data());
		std::string crypTNewPw = Crypter::SimpleHash(newPw.data());
		if(!m_currentUser->changePassword(cryptCurPw, crypTNewPw)) {
			Message::error(NULL, "Senha atual incorreta.");
			return;
		}

		g_cinetico.cineticoDB()->userProfileDAO()->update(*m_currentUser);
		Message::msg(NULL,"Senha alterada com sucesso.");

		tbOldPassword.setText("");
		tbNewPassword.setText("");
		tbNewPasswordConfirmation.setText("");
	}

}