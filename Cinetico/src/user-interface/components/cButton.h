#ifndef __CINETICO_BUTTON_H__
#define __CINETICO_BUTTON_H__

#include "uilib/ui/uibase.h"

namespace cinetico {
	
	using namespace uilib;

	class cButton : public Label {
		virtual void onMouseEnterEvent(MouseEvent &event);
		virtual void onMouseLeaveEvent(MouseEvent &event);
		virtual void onMouseReleaseEvent(MouseEvent &event);

	public:
		cButton();

	};

}

#endif