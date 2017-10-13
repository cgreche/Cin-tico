
#ifndef __CINETICO_VIEWTEMPLATE_H__
#define __CINETICO_VIEWTEMPLATE_H__

#include "uilib/ui/uibase.h"

namespace cinetico {

	using namespace uilib;

	class ViewTemplate {
	public:
		static FontDesc AppTitleFont;
		static Color AppTitleColor;
		static Color AppHeaderBgColor;
		static FontDesc FooterInfoFont;
		static Color FooterInfoColor;
		static FontDesc LoggedUserInfoFont;
		static Color LoggedUserInfoColor;
		static Color LoggedUserInfoHoverColor;
		static FontDesc TitleFont;
		static Color TitleColor;
		static FontDesc DescFont;
		static Color DescColor;
		static FontDesc SectionSeparatorFont;
		static Color SectionSeparatorTextColor;
		static Color SectionSeparatorLineColor;
		static FontDesc ControlsLabelFont;
		static Color ControlsLabelColor;
		static FontDesc TextLinkFont;
		static Color TextLinkColor;
		static Color TextLinkHoverColor;
	};

}

#endif