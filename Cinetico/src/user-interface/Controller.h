
#ifndef __CINETICO_CONTROLLER_H__
#define __CINETICO_CONTROLLER_H__

#include "uilib/ui/uibase.h"

namespace cinetico {

	class Controller
	{
	public:
		virtual Layout *viewDefinition() = 0;
		virtual void onViewEnter() { }
		virtual void onViewTick() { }
		virtual void onViewQuit() { }
	};

}

#endif