
#ifndef __CINETICO_MAINWINDOW_H__
#define __CINETICO_MAINWINDOW_H__

#include <vector>

#include "uilib/ui/uibase.h"

namespace cinetico {

	class MainWindow : public Window
	{
		VerticalLayout layout;
		VerticalLayout layoutHeader;
			HorizontalLayout layoutAppState;
				Label labelAppname;
				HorizontalLayout layoutLoginInfo;
					Label labelUsername;
					Label buttonLogoff;

		VerticalLayout layoutContent;
		VerticalLayout layoutFooter;
		Label labelAuthor;

		Layout *m_currentContentLayout;

	public:
		void buildHeaderLayout();
		void buildFooterLayout();
		MainWindow();
		void setContentLayout(Layout *layout);
		void onCloseEvent();
	};

}

#endif