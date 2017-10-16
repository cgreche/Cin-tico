
#include "cPageTitle.h"
#include "../viewtemplate.h"

namespace cinetico {
	
	cPageTitle::cPageTitle() {

		FontDesc &titleFont = ViewTemplate::TitleFont;
		Color &titleColor = ViewTemplate::TitleColor;
		FontDesc &descFont = ViewTemplate::DescFont;
		Color &descColor = ViewTemplate::DescColor;

		labelTitle.setFont(titleFont);
		labelTitle.setTextColor(titleColor);
		labelDescr.setFont(descFont);
		labelDescr.setTextColor(descColor);

		append(labelTitle);
		append(labelDescr);
		append(separator);
	}

	void cPageTitle::setTitle(const string &title) {
		labelTitle.setText(title);
	}

	void cPageTitle::setDesc(const string &desc) {
		labelDescr.setText(desc);
	}

}