
#ifndef __CINETICO_CONTROLLER_H__
#define __CINETICO_CONTROLLER_H__

#include "uilib/ui/uibase.h"
#include <map>

namespace cinetico {

	class Cinetico;
	class CineticoUI;
	class Dictionary;

	class Controller
	{
	protected:
		Cinetico &m_cinetico;
		CineticoUI &m_cineticoUI;
		Dictionary &m_dictionary;

	public:
		typedef std::map<const char*, void*> ViewParams;
		Controller(CineticoUI &cineticoUI);
		virtual uilib::Layout *viewDefinition() = 0;
		virtual void onViewEnter(ViewParams *params);
		virtual void onViewUpdate();
		virtual void onViewTick();
		virtual void onViewQuit();
	};

}

#endif