
#include "mainwindow.h"

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

	void MainWindow::buildHeaderLayout() {
		labelAppname.setText("Cin�tico");
		labelAppname.setTextColor(Color(0x80, 0x80, 0x80));
		labelAppname.setAlignment(Label::VCenter);
		labelAppname.setFont(FontDesc("Arial", 16, FONT_BOLD));

		labelUsername.setText("Usu�rio: ");
		labelUsername.setTextColor(Color(255, 255, 255));
		linkUsername.setTextColor(Color(255, 255, 255));
		linkUsername.setHoverColor(Color(200, 0, 0));
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

		bgBlack.setBackgroundColor(Color(0, 0, 0));
		bgBlack.setTransparent(false);
		bgBlack.setLayout(&layoutAppState);
		layoutHeader.append(bgBlack, Size(SizeTypeMax, SizeTypeMax));
	}

	void MainWindow::buildFooterLayout() {
		labelAuthor.setText("2017 C�sar Reche");
		labelAuthor.setTransparent(false);
		layoutFooter.append(labelAuthor);
	}

	MainWindow::MainWindow(Cinetico &cinetico)
	: m_cinetico(cinetico) {
		m_currentContentLayout = NULL;

		setTitle("Cin�tico");
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
		UserProfile *currentUser = m_cinetico.currentUser();
		if (lastUserProfile != currentUser) {
			if (currentUser) {
				linkUsername.setText(currentUser->loginName().c_str());
				layoutLoginInfo.setVisible(true);
				layoutHeader.setSize(layoutHeader.size());
			}
			else {
				layoutLoginInfo.setVisible(false);
			}
			lastUserProfile = currentUser;
		}
	}

	void MainWindow::onClickUserLoginName() {
		Controller::ViewParams params;
		params["user"] = m_cinetico.currentUser();
		m_cinetico.goTo(Cinetico::USER_PROFILE,&params);
	}

	void MainWindow::onClickLogoff() {
		m_cinetico.logoffCurrentUser();
		m_cinetico.goTo(Cinetico::LOGIN);
	}

	void MainWindow::onCloseEvent() {
		this->setVisible(false);
		uibase::UIQuit();
	}

}