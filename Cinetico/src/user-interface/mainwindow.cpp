
#include "cineticoui.h"
#include "mainwindow.h"
#include "input.h"
#include "dictionary.h"
#include "entity/UserProfile.h"
#include "viewtemplate.h"
#include "playmodes/PlayMode.h"

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

	static void buttonGoToDebugView_onClick(Button &button) {
		MainWindow *mainWindow = (MainWindow *)button.param();
		Controller::ViewParams params;
		mainWindow->m_cinetico.cineticoUI()->goTo(CineticoUI::DEBUG, params);
	}

	static void buttonGoToDebugMode_onClick(Button &button) {
		MainWindow *mainWindow = (MainWindow *)button.param();
		Controller::ViewParams params;
		params.set<int>("play_mode", PlayMode::DEBUG_MODE);
		mainWindow->m_cinetico.cineticoUI()->goTo(CineticoUI::PLAYING,params);
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

		layoutHeader.append(layoutAppState);
		layoutHeader.setAlignment(Layout::center_align);

		bgHeader.setBackgroundColor(ViewTemplate::AppHeaderBgColor);
		bgHeader.setTransparent(false);
	}

	void MainWindow::buildFooterLayout() {
		labelAuthor.setText("2017 César Reche");
		labelAuthor.setFont(ViewTemplate::FooterInfoFont);
		labelAuthor.setTextColor(ViewTemplate::FooterInfoColor);

		buttonDebugView.setText("Go to Debug view");
		buttonDebugView.setParam(this);
		buttonDebugView.setOnClick(buttonGoToDebugView_onClick);

		buttonDebugMode.setText("Go to debug mode");
		buttonDebugMode.setParam(this);
		buttonDebugMode.setOnClick(buttonGoToDebugMode_onClick);
		layoutOptions.append(buttonDebugView);
		layoutOptions.append(buttonDebugMode);
		layoutOptions.setAlignment(Layout::center_align);
		
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

		layoutAuthorInfo.append(labelAuthor);

		layoutLanguages.append(linkPortugues);
		layoutLanguages.append(linkEnglish);
		layoutLanguages.append(linkEspanol);
		layoutLanguages.setAlignment(Layout::right_align);

		layoutFooter.append(layoutAuthorInfo, MaximumSize);
		layoutFooter.append(layoutOptions, MaximumSize);
		layoutFooter.append(layoutLanguages, MaximumSize);
		layoutFooter.setMargin(10);

		bgHeader.setLayout(&layoutHeader);
		bgContent.setLayout(&layoutContent);
		bgFooter.setLayout(&layoutFooter);
	}


	void MainWindow::onKeyPressEvent(KeyEvent &event) {
		m_cinetico.input()->keyboard.m_keys[event.key()] = true;
	}
	
	void MainWindow::onKeyReleaseEvent(KeyEvent &event) {
		m_cinetico.input()->keyboard.m_keys[event.key()] = false;
	}

	void MainWindow::onMouseMoveEvent(MouseEvent &event) {
		m_cinetico.input()->mouse.m_x = event.position().x();
		m_cinetico.input()->mouse.m_y = event.position().y();
	}

	void MainWindow::onMousePressEvent(MouseEvent &event) {
		uilib::u32 button = event.button();
		if (button == 1)
			m_cinetico.input()->mouse.m_buttons[0] = true;
		else if(button == 2)
			m_cinetico.input()->mouse.m_buttons[1] = true;
		else if(button == 4)
			m_cinetico.input()->mouse.m_buttons[2] = true;
	}

	void MainWindow::onMouseReleaseEvent(MouseEvent &event) {
		uilib::u32 button = event.button();
		if (button == 1)
			m_cinetico.input()->mouse.m_buttons[0] = false;
		else if (button == 2)
			m_cinetico.input()->mouse.m_buttons[1] = false;
		else if (button == 4)
			m_cinetico.input()->mouse.m_buttons[2] = false;
	}

	MainWindow::MainWindow(Cinetico &cinetico)
		: m_cinetico(cinetico) {
		m_currentContentLayout = NULL;

		setTitle("Cinético");
		setStyle(CS_Window | CS_Resizable | CS_Caption | CS_SysMenu | CS_MinimizeButton | CS_MaximizeButton);
		buildHeaderLayout();
		buildFooterLayout();

		layout.append(bgHeader, Size(SizeTypeMax, MakePercentType(10)), 20);
		layout.append(bgContent, MaximumSize, 20);
		layout.append(bgFooter, Size(SizeTypeMax, MakePercentType(10)));

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

	void MainWindow::step() {
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
		bgHeader.setVisible(visible);
		if (!visible)
			layout.remove(bgHeader);
		else
			layout.insertBefore(bgContent, bgHeader, Size(SizeTypeMax, MakePercentType(10)), 20);
	}

	void MainWindow::setContentVisible(bool visible) {
		bgContent.setVisible(visible);
		if (!visible)
			layout.remove(bgContent);
		else
			layout.insertBefore(bgFooter, bgContent, MaximumSize, 20);
	}

	void MainWindow::setFooterVisible(bool visible) {
		bgFooter.setVisible(visible);
		if (!visible)
			layout.remove(bgFooter);
		else
			layout.insertAfter(bgContent, bgFooter, Size(SizeTypeMax, MakePercentType(10)));
	}

	void MainWindow::onClickUserLoginName() {
		Controller::ViewParams params;
		params.set<UserProfile*>("user",m_cinetico.currentUser());
		m_cinetico.cineticoUI()->goTo(CineticoUI::USER_PROFILE,params);
	}

	void MainWindow::onClickLogoff() {
		m_cinetico.logoffCurrentUser();
		m_cinetico.cineticoUI()->goTo(CineticoUI::LOGIN);
	}

	void MainWindow::onCloseEvent() {
		this->setVisible(false);
		UI::UIQuit();
	}

}