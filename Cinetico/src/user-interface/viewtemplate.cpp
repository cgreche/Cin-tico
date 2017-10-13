
#include "viewtemplate.h"

namespace cinetico {

	using namespace uilib;

	FontDesc ViewTemplate::AppTitleFont = FontDesc("Arial", 16, FONT_BOLD);
	Color ViewTemplate::AppTitleColor = Color(0x80, 0x80, 0x80);
	Color ViewTemplate::AppHeaderBgColor = Color(0, 0, 0);
	FontDesc ViewTemplate::FooterInfoFont = FontDesc("Arial", 10, 0);
	Color ViewTemplate::FooterInfoColor = Color(0,0,0);
	FontDesc ViewTemplate::LoggedUserInfoFont = FontDesc("Arial", 10, 0);
	Color ViewTemplate::LoggedUserInfoColor = Color(255, 255, 255);
	Color ViewTemplate::LoggedUserInfoHoverColor = Color(200, 0, 0);
	FontDesc ViewTemplate::TitleFont = FontDesc("Arial", 46, FONT_BOLD);
	Color ViewTemplate::TitleColor = Color(0,0,0);
	FontDesc ViewTemplate::DescFont = FontDesc("Arial", 10, FONT_BOLD);
	Color ViewTemplate::DescColor = Color(0,0,0);
	FontDesc ViewTemplate::SectionSeparatorFont = FontDesc("Arial", 12, FONT_BOLD);
	Color ViewTemplate::SectionSeparatorTextColor = Color(0,0,0);
	Color ViewTemplate::SectionSeparatorLineColor = Color(0,0,0);
	FontDesc ViewTemplate::ControlsLabelFont = FontDesc("Arial", 10, 0);
	Color ViewTemplate::ControlsLabelColor = Color(0,0,0);
	FontDesc ViewTemplate::TextLinkFont = FontDesc("Arial", 10, 0);
	Color ViewTemplate::TextLinkColor = Color(0,0,0);
	Color ViewTemplate::TextLinkHoverColor = Color(255,120,0);
}
