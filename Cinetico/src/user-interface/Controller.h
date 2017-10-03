
#ifndef __CINETICO_CONTROLLER_H__
#define __CINETICO_CONTROLLER_H__

#include "uilib/ui/uibase.h"
#include <map>

namespace cinetico {

	class Controller
	{
	public:
		typedef std::map<const char*, void*> ViewParams;
		virtual uilib::Layout *viewDefinition() = 0;
		virtual void onViewEnter(ViewParams *params) { }
		virtual void onViewTick() { }
		virtual void onViewQuit() { }
	};

}

#endif