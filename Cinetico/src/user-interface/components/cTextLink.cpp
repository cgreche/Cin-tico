
#include "cTextLink.h"
#include "../viewtemplate.h"

namespace cinetico {
	
	cTextLink::cTextLink() {
		FontDesc &font = ViewTemplate::TextLinkFont;
		FontDesc &hoverFont = ViewTemplate::TextLinkHoverFont;
		Color &color = ViewTemplate::TextLinkColor;
		Color &hoverColor = ViewTemplate::TextLinkHoverColor;

		setFont(font);
		setTextColor(color);
		setHoverFont(hoverFont);
		setHoverColor(hoverColor);
	}

}