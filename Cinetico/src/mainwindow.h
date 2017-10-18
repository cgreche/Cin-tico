
#ifndef __CINETICO_MAINWINDOW_H__
#define __CINETICO_MAINWINDOW_H__

#include <vector>

#include "cinetico.h"
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
		void onLanguageSelect(Dictionary::LanguageID langId);

	public:
		MainWindow(Cinetico &cinetico);
		void update();
		void setContentLayout(Layout *layout);
		
		void onCloseEvent();
	};

}

#endif