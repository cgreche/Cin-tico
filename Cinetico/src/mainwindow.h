
#ifndef __CINETICO_MAINWINDOW_H__
#define __CINETICO_MAINWINDOW_H__

#include <vector>
#include "uilib/ui/uibase.h"

namespace cinetico {

	using namespace uilib;
	class Cinetico;

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
						TextLink linkUsername;
						Button buttonLogoff;

		VerticalLayout layoutContent;
		HorizontalLayout layoutFooter;
			Label labelAuthor;
			VerticalLayout layoutLanguages;
				TextLink linkPortugues;
				TextLink linkEnglish;
				TextLink linkEspanol;

		Layout *m_currentContentLayout;

		void buildHeaderLayout();
		void buildFooterLayout();

		Cinetico &m_cinetico;

		friend void linkUserLoginName_onClick(TextLink &link);
		friend void buttonLogoff_onClick(Button &button);
		friend void linkPortugues_onClick(TextLink &link);
		friend void linkEnglish_onClick(TextLink &link);
		friend void linkEspanol_onClick(TextLink &link);
		void onClickUserLoginName();
		void onClickLogoff();

		//
		virtual void onCloseEvent();
		virtual void onKeyPressEvent(KeyEvent &event);
		virtual void onKeyReleaseEvent(KeyEvent &event);
		virtual void onMouseMoveEvent(MouseEvent &event);
		virtual void onMousePressEvent(MouseEvent &event);
		virtual void onMouseReleaseEvent(MouseEvent &event);

	public:
		MainWindow(Cinetico &cinetico);
		void step();
		void setContentLayout(Layout *layout);
		void setHeaderVisible(bool visible);
		void setFooterVisible(bool visible);
	};

}

#endif