
#include "cineticoui.h"
#include "mainwindow.h"
#include "dictionary.h"
#include "entity/user/UserProfile.h"
#include "user-interface/viewtemplate.h"

using namespace cinetico;

namespace cinetico {

	static void linkUserLoginName_onClick(TextLink &link) {
		MainWindow* mainWindow = (MainWindow *)link.param();
		mainWindow->onClickUserLoginName();
	}

	static void buttonLogoff_onClick(Button &button) {
		MainWindow* mainWindow = (MainWindow *)button.param();
		mainWindow->onClickLogoff();
	}
	
	static void linkPortugues_onClick(TextLink &link) {
		MainWindow* mainWindow = (MainWindow *)link.param();
		mainWindow->m_cinetico.setLanguage(Dictionary::PT_BR);
	}

	static void linkEnglish_onClick(TextLink &link) {
		MainWindow* mainWindow = (MainWindow *)link.param();
		mainWindow->m_cinetico.setLanguage(Dictionary::EN_US);
	}

	static void linkEspanol_onClick(TextLink &link) {
		MainWindow* mainWindow = (MainWindow *)link.param();
		mainWindow->m_cinetico.setLanguage(Dictionary::ES_ES);
	}

	void MainWindow::buildHeaderLayout() {
		labelAppname.setText("Cinético");
		labelAppname.setTextColor(ViewTemplate::AppTitleColor);
		labelAppname.setAlignment(Label::VCenter);
		labelAppname.setFont(ViewTemplate::AppTitleFont);

		labelUsername.setText("Usuário: ");
		labelUsername.setTextColor(ViewTemplate::LoggedUserInfoColor);
		linkUsername.setTextColor(ViewTemplate::LoggedUserInfoColor);
		linkUsername.setHoverColor(ViewTemplate::LoggedUserInfoHoverColor);
		linkUsername.setParam(this);
		linkUsername.setOnClick(linkUserLoginName_onClick);
		buttonLogoff.setText("Sair");
		buttonLogoff.setParam(this);
		buttonLogoff.setOnClick(buttonLogoff_onClick);

		layoutLoginInfo2.append(labelUsername);
		layoutLoginInfo2.append(linkUsername);
		layoutLoginInfo2.append(buttonLogoff);
		layoutLoginInfo2.setAlignment(Layout::center_align);
		layoutLoginInfo.append(layoutLoginInfo2);
		layoutLoginInfo.setAlignment(Layout::right_align);
		layoutLoginInfo.setVisible(false);

		layoutAppState.append(labelAppname);
		layoutAppState.append(layoutLoginInfo,Size(SizeTypeMax,SizeTypeAuto));
		layoutAppState.setAlignment(Layout::center_align);
		layoutAppState.setMargin(10);

		bgBlack.setBackgroundColor(ViewTemplate::AppHeaderBgColor);
		bgBlack.setTransparent(false);
		bgBlack.setLayout(&layoutAppState);
		layoutHeader.append(bgBlack, Size(SizeTypeMax, SizeTypeMax));
	}

	void MainWindow::buildFooterLayout() {
		labelAuthor.setText("2017 César Reche");
		labelAuthor.setFont(ViewTemplate::FooterInfoFont);
		labelAuthor.setTextColor(ViewTemplate::FooterInfoColor);
		
		linkPortugues.setText("Português");
		linkPortugues.setTextColor(ViewTemplate::TextLinkColor);
		linkPortugues.setHoverColor(ViewTemplate::TextLinkHoverColor);
		linkPortugues.setParam(this);
		linkPortugues.setOnClick(linkPortugues_onClick);
		
		linkEnglish.setText("English");
		linkEnglish.setTextColor(ViewTemplate::TextLinkColor);
		linkEnglish.setHoverColor(ViewTemplate::TextLinkHoverColor);
		linkEnglish.setParam(this);
		linkEnglish.setOnClick(linkEnglish_onClick);

		linkEspanol.setText("Español");
		linkEspanol.setTextColor(ViewTemplate::TextLinkColor);
		linkEspanol.setHoverColor(ViewTemplate::TextLinkHoverColor);
		linkEspanol.setParam(this);
		linkEspanol.setOnClick(linkEspanol_onClick);

		layoutLanguages.append(linkPortugues);
		layoutLanguages.append(linkEnglish);
		layoutLanguages.append(linkEspanol);
		layoutLanguages.setAlignment(Layout::right_align);

		layoutFooter.setMargin(10);
		layoutFooter.append(labelAuthor, MaximumSize);
		layoutFooter.append(layoutLanguages, MaximumSize);
	}

	MainWindow::MainWindow(Cinetico &cinetico)
		: m_cinetico(cinetico) {
		m_currentContentLayout = NULL;

		setTitle("Cinético");
		setStyle(CS_Window | CS_Resizable | CS_Caption | CS_SysMenu | CS_MinimizeButton | CS_MaximizeButton);
		buildHeaderLayout();
		buildFooterLayout();

		layout.append(layoutHeader, Size(SizeTypeMax, MakePercentType(10)), 20);
		layout.append(layoutContent, MaximumSize, 20);
		layout.append(layoutFooter, Size(SizeTypeMax, MakePercentType(10)));

		setLayout(&layout);
		setFrameSize(Size(1024, 768));
		setVisible(true);
	}

	void MainWindow::setContentLayout(Layout *layout) {
		if (m_currentContentLayout)
			layoutContent.remove(*m_currentContentLayout);
		if (layout) {
			layoutContent.append(*layout);
			layout->setVisible(true); //todo: set layout visibility internally
		}
		m_currentContentLayout = layout;
	}

	void MainWindow::update() {
		static UserProfile *lastUserProfile = NULL;
		static std::string username = "";
		std::string usernameMessage;

		UserProfile *currentUser = m_cinetico.currentUser();
		if (lastUserProfile != currentUser) {
			if (currentUser) {
				username = currentUser->name();
				if (*username.c_str()) {
					usernameMessage = username;
					usernameMessage += " (";
					usernameMessage += currentUser->loginName();
					usernameMessage += ")";
				}
				else {
					usernameMessage = currentUser->loginName().c_str();
				}
				linkUsername.setText(usernameMessage.c_str());
				layoutLoginInfo.setVisible(true);
				layoutHeader.setSize(layoutHeader.size());
			}
			else {
				layoutLoginInfo.setVisible(false);
			}
			lastUserProfile = currentUser;
		}
		else {
			if (currentUser) {
				if (username != currentUser->name()) {
					username = currentUser->name();
					if (*username.c_str()) {
						usernameMessage = username;
						usernameMessage += " (";
						usernameMessage += currentUser->loginName();
						usernameMessage += ")";
					}
					else {
						usernameMessage = currentUser->loginName().c_str();
					}
					linkUsername.setText(usernameMessage.c_str());
					layoutHeader.setSize(layoutHeader.size());
				}
			}
		}
	}

	void MainWindow::setHeaderVisible(bool visible) {
		layoutHeader.setVisible(visible);
		if (!visible)
			layout.remove(layoutHeader);
		else
			layout.insertBefore(layoutContent, layoutHeader);
	}

	void MainWindow::setFooterVisible(bool visible) {
		layoutFooter.setVisible(visible);
		if (!visible)
			layout.remove(layoutFooter);
		else
			layout.insertAfter(layoutContent, layoutFooter);
	}


	void MainWindow::onClickUserLoginName() {
		Controller::ViewParams params;
		params["user"] = m_cinetico.currentUser();
		m_cinetico.cineticoUI()->goTo(CineticoUI::USER_PROFILE,&params);
	}

	void MainWindow::onClickLogoff() {
		m_cinetico.logoffCurrentUser();
		m_cinetico.cineticoUI()->goTo(CineticoUI::LOGIN);
	}

	void MainWindow::onCloseEvent() {
		this->setVisible(false);
		uibase::UIQuit();
	}


}