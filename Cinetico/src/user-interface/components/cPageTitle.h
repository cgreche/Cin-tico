#ifndef __CINETICO_PAGETITLE_H__
#define __CINETICO_PAGETITLE_H__

#include "uilib/ui/uibase.h"

namespace cinetico {
	
	using namespace uilib;

	class cPageTitle : public VerticalLayout {

	public:
		Label labelTitle;
		Label labelDescr;
		Separator separator;

		cPageTitle(const char *title, const char *desc);

	};

}

#endif