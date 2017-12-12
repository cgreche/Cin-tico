#ifndef __CINETICO_BUTTON_H__
#define __CINETICO_BUTTON_H__

#include "uilib/ui/uibase.h"

namespace cinetico {
	
	using namespace uilib;

	class cButton : public Label {
	public:
		typedef void(*cButtonActionFunc)(cButton& button);

	private:
		virtual void onMouseEnterEvent(MouseEvent &event);
		virtual void onMouseLeaveEvent(MouseEvent &event);
		virtual void onMousePressEvent(MouseEvent &event);
		virtual void onMouseReleaseEvent(MouseEvent &event);

		std::vector<cButtonActionFunc> m_callbacks;

	public:
		cButton();
		void addOnAction(cButtonActionFunc callback) {
			m_callbacks.push_back(callback);
		}

		virtual Size getAutoSize();

	};

}

#endif