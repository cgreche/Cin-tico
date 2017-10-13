
#ifndef __CINETICO_VIEWTEMPLATE_H__
#define __CINETICO_VIEWTEMPLATE_H__

#include "uilib/ui/uibase.h"

namespace cinetico {

	using namespace uilib;

	class ViewTemplate {
	public:
		static FontDesc AppTitleFont;
		static uilib::Color AppTitleColor;
		static uilib::Color AppHeaderBgColor;
		static FontDesc FooterInfoFont;
		static uilib::Color FooterInfoColor;
		static FontDesc LoggedUserInfoFont;
		static uilib::Color LoggedUserInfoColor;
		static uilib::Color LoggedUserInfoHoverColor;
		static FontDesc TitleFont;
		static uilib::Color TitleColor;
		static FontDesc DescFont;
		static uilib::Color DescColor;
		static FontDesc SectionSeparatorFont;
		static uilib::Color SectionSeparatorTextColor;
		static uilib::Color SectionSeparatorLineColor;
		static FontDesc ControlsLabelFont;
		static uilib::Color ControlsLabelColor;
		static FontDesc TextLinkFont;
		static uilib::Color TextLinkColor;
		static uilib::Color TextLinkHoverColor;
	};

}

#endif