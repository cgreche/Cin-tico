
#include "cSeparator.h"
#include "../viewtemplate.h"

namespace cinetico {

	cSeparator::cSeparator() {
		FontDesc& sepFont = ViewTemplate::SectionSeparatorFont;
		Color& sepTextColor = ViewTemplate::SectionSeparatorTextColor;
		Color& sepLineColor = ViewTemplate::SectionSeparatorLineColor;

		setFont(sepFont);
		setTextColor(sepTextColor);
		setLineColor(sepLineColor);
	}

}
