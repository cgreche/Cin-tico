
#include "mainwindow.h"

using namespace cinetico;

namespace cinetico {

	static void buttonLogoff_onClick(Button &button) {
		MainWindow* mainWindow = (MainWindow *)button.param();
		mainWindow->onClickLogoff();
	}

	void MainWindow::buildHeaderLayout() {
		labelAppname.setText("Cinético");
		labelAppname.setTextColor(Color(0x80, 0x80, 0x80));
		labelAppname.setAlignment(Label::VCenter);
		labelAppname.setFont(FontDesc("Arial", 16, FONT_BOLD));

		labelUsername.setTextColor(Color(255, 255, 255));
		buttonLogoff.setText("Sair");
		buttonLogoff.setParam(this);
		buttonLogoff.setOnClick(buttonLogoff_onClick);

		layoutLoginInfo2.append(labelUsername);
		layoutLoginInfo2.append(buttonLogoff);
		layoutLoginInfo2.setAlignment(Layout::center_align);
		layoutLoginInfo.append(layoutLoginInfo2);
		layoutLoginInfo.setAlignment(Layout::right_align);
		layoutLoginInfo.setVisible(false);

		layoutAppState.append(labelAppname);
		layoutAppState.append(layoutLoginInfo,MaximumSize);
		layoutAppState.setAlignment(Layout::center_align);

		bgBlack.setBackgroundColor(Color(0, 0, 0));
		bgBlack.setTransparent(false);
		bgBlack.setLayout(&layoutAppState);
		layoutHeader.append(bgBlack, Size(SizeTypeMax, SizeTypeMax));
	}

	void MainWindow::buildFooterLayout() {
		labelAuthor.setText("2017 César Reche");
		labelAuthor.setTransparent(false);
		layoutFooter.append(labelAuthor);
	}

	MainWindow::MainWindow(Cinetico &cinetico)
	: m_cinetico(cinetico) {
		m_currentContentLayout = NULL;

		setTitle("Cinético");
		setStyle(CS_Window | CS_Resizable | CS_Caption | CS_SysMenu | CS_MinimizeButton | CS_MaximizeButton);
		buildHeaderLayout();
		buildFooterLayout();

		layout.append(layoutHeader, Size(SizeTypeMax, MakePercentType(10)), 20);
		layout.append(layoutContent, AutoSize, 20);
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
				labelUsername.setText(currentUser->username().c_str());
				layoutLoginInfo.setVisible(true);
				layoutHeader.setSize(layoutHeader.size());
			}
			else {
				layoutLoginInfo.setVisible(false);
			}
			lastUserProfile = currentUser;
		}
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