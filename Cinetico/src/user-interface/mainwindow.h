
#ifndef __CINETICO_MAINWINDOW_H__
#define __CINETICO_MAINWINDOW_H__

#include <vector>
#include "uilib/ui/uibase.h"
#include "components/cTextLink.h"
#include "components/cButton.h"

namespace cinetico {

	using namespace uilib;
	class Cinetico;
	class CineticoUI;

	class MainWindow : public Window
	{
		VerticalLayout layout;
		Label bgHeader;
			VerticalLayout layoutHeader;
			HorizontalLayout layoutAppState;
				Label labelAppname;

				//Login Info
				VerticalLayout layoutLoginInfo;
					HorizontalLayout layoutLoginInfo2;
						Label labelUsername;
						TextLink linkUsername;
						cButton buttonLogoff;

		Label bgContent;
		VerticalLayout layoutContent;

		Label bgFooter;
		HorizontalLayout layoutFooter;
			VerticalLayout layoutAuthorInfo;
				Label labelAuthor;
			VerticalLayout layoutOptions;
				Button buttonConfig;
				Button buttonDebugView;
				Button buttonDebugMode;
			VerticalLayout layoutLanguages;
				cTextLink linkPortugues;
				cTextLink linkEnglish;
				cTextLink linkEspanol;

		Layout *m_currentContentLayout;

		void buildHeaderLayout();
		void buildFooterLayout();
		void updateLabels();

		CineticoUI &m_cineticoUI;
		Cinetico &m_cinetico;

		friend void linkUserLoginName_onClick(TextLink &link);
		friend void buttonLogoff_onClick(cButton &button);
		friend void linkPortugues_onClick(TextLink &link);
		friend void linkEnglish_onClick(TextLink &link);
		friend void linkEspanol_onClick(TextLink &link);
		friend void buttonConfig_onClick(Button &button);
		friend void buttonGoToDebugView_onClick(Button &button);
		friend void buttonGoToDebugMode_onClick(Button &button);
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
		MainWindow(CineticoUI &cineticoUI);
		void step();
		void update();
		void setContentLayout(Layout *layout);
		void setHeaderVisible(bool visible);
		void setContentVisible(bool visible);
		void setFooterVisible(bool visible);
	};

}

#endif