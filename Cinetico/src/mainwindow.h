
#ifndef __CINETICO_MAINWINDOW_H__
#define __CINETICO_MAINWINDOW_H__

#include <vector>

#include "cinetico.h"
#include "uilib/ui/uibase.h"

namespace cinetico {

	using namespace uilib;

	class MainWindow : public Window
	{
		VerticalLayout layout;
		VerticalLayout layoutHeader;
			Label bgBlack;
			HorizontalLayout layoutAppState;
				Label labelAppname;
				VerticalLayout layoutLoginInfo;
					HorizontalLayout layoutLoginInfo2;
						Label labelUsername;
						Button buttonLogoff;

		VerticalLayout layoutContent;
		VerticalLayout layoutFooter;
		Label labelAuthor;

		Layout *m_currentContentLayout;

		void buildHeaderLayout();
		void buildFooterLayout();

		Cinetico &m_cinetico;

		friend void buttonLogoff_onClick(Button &button);
		void onClickLogoff();

	public:
		MainWindow(Cinetico &cinetico);
		void update();
		void setContentLayout(Layout *layout);
		
		void onCloseEvent();
	};

}

#endif