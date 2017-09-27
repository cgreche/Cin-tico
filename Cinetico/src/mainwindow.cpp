#include <windows.h>
#include <math.h>
#include <time.h>

#include "mainwindow.h"

using namespace cinetico;

namespace cinetico {

	void MainWindow::buildHeaderLayout() {
		labelAppname.setText("Cinético");
		labelAppname.setTextColor(Color(0x80, 0x80, 0x80));
		labelAppname.setAlignment(Label::VCenter);
		labelAppname.setBackgroundColor(Color(0, 0, 0));
		labelAppname.setTransparent(false);
		labelAppname.setFont(FontDesc("Arial", 16, FONT_BOLD));

		layoutHeader.append(labelAppname, Size(SizeTypeMax, SizeTypeMax));
	}

	void MainWindow::buildFooterLayout() {
		labelAuthor.setText("2017 César Reche");
		labelAuthor.setTransparent(false);
		layoutFooter.append(labelAuthor);
	}

	MainWindow::MainWindow() {
		m_currentContentLayout = NULL;

		setTitle("Cinético");
		setStyle(CS_Window | CS_Resizable | CS_Caption | CS_SysMenu | CS_MinimizeButton | CS_MaximizeButton);
		buildHeaderLayout();
		buildFooterLayout();

		layout.append(layoutHeader, Size(SizeTypeMax, MakePercentType(10)), 20);
		layout.append(layoutContent, AutoSize, 20);
		layout.append(layoutFooter, Size(SizeTypeMax, MakePercentType(20)));

		setLayout(&layout);
		setFrameSize(Size(1024, 768));
		setVisible(true);
	}

	void MainWindow::setContentLayout(Layout *layout) {
		if (m_currentContentLayout)
			layoutContent.remove(*m_currentContentLayout);
		if (layout)
			layoutContent.append(*layout);
		m_currentContentLayout = layout;
	}

	void MainWindow::onCloseEvent() {
		this->setVisible(false);
		uibase::UIQuit();
	}

}