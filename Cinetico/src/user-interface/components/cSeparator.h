#ifndef __CINETICO_SEPARATOR_H__
#define __CINETICO_SEPARATOR_H__

#include "uilib/ui/uibase.h"

namespace cinetico {
	
	using namespace uilib;

	class cSeparator : public uilib::Separator {

	public:
		uilib::Separator separator;
		cSeparator();
	};

}

#endif