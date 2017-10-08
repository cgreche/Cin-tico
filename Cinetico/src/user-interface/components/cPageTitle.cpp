
#include "cPageTitle.h"

namespace cinetico {
	
	cPageTitle::cPageTitle(const char *title, const char *descr) {

		FontDesc titleFont = FontDesc("Arial", 46, FONT_BOLD);
		FontDesc descFont = FontDesc("Arial", 10, FONT_BOLD);

		labelTitle.setText(title);
		labelTitle.setFont(titleFont);
		labelDescr.setText(descr);
		labelDescr.setFont(descFont);

		append(labelTitle);
		append(labelDescr);
		append(separator);
	}

}