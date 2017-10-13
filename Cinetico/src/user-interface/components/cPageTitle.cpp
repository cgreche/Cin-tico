
#include "cPageTitle.h"
#include "../viewtemplate.h"

namespace cinetico {
	
	cPageTitle::cPageTitle(const char *title, const char *descr) {

		FontDesc &titleFont = ViewTemplate::TitleFont;
		Color &titleColor = ViewTemplate::TitleColor;
		FontDesc &descFont = ViewTemplate::DescFont;
		Color &descColor = ViewTemplate::DescColor;

		labelTitle.setText(title);
		labelTitle.setFont(titleFont);
		labelTitle.setTextColor(titleColor);
		labelDescr.setText(descr);
		labelDescr.setFont(descFont);
		labelDescr.setTextColor(descColor);

		append(labelTitle);
		append(labelDescr);
		append(separator);
	}

}