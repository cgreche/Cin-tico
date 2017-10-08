#ifndef __CINETICO_TEXTBOX_H__
#define __CINETICO_TEXTBOX_H__

#include "uilib/ui/uibase.h"

namespace cinetico {
	
	using namespace uilib;

	class cTextBox : public VerticalLayout {

	public:
		Label label;
		EditBox edit;

		cTextBox() {
			append(label);
			append(edit);
		}

		void setLabel(const char *text) { label.setText(text); }
		void setText(const char *text) { edit.setText(text); }
		void setPasswordMode(bool passwordMode) { edit.setPasswordMode(passwordMode); }
		string &text() { return edit.text(); }
		bool passwordMode() const { return edit.passwordMode(); }

	};

}

#endif