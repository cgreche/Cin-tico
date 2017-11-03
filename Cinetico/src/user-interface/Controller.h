
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
		class ViewParams {
			std::map<const char *, void*> m_map;
		public:
			template <class T> void set(const char *key, T value) {
				m_map[key] = (void*)value;
			}
			template <class T> T get(const char *key) {
				std::map<const char*,void*>::iterator it = m_map.find(key);
				if (it == m_map.end())
					return (T)0;
				return (T)it->second;
			}
		};

		Controller(CineticoUI &cineticoUI);
		virtual uilib::Layout *viewDefinition() = 0;
		virtual void onViewEnter(ViewParams params);
		virtual void onViewUpdate();
		virtual void onViewTick();
		virtual void onViewQuit();
	};

}

#endif