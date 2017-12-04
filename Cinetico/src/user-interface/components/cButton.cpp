
#include "cButton.h"
#include "../viewtemplate.h"


namespace cinetico {
	
	void cButton::onMouseEnterEvent(MouseEvent &event) {
		setBackgroundColor(Color(0x80, 0x80, 0x80));
		setTextColor(Color(0x80, 0x80, 0x80));
	}

	void cButton::onMouseLeaveEvent(MouseEvent &event) {
		setBackgroundColor(Color(255, 255, 255));
		setTextColor(Color(0x50, 0x50, 0x50));
		m_parentLayout->update();
	}

	void cButton::onMouseReleaseEvent(MouseEvent &event) {
		for (cButtonActionFunc f : m_callbacks) {
			(*f)(*this);
		}
	}

	cButton::cButton() {
		FontDesc &controlsLabelFont = ViewTemplate::ControlsLabelFont;
		Color &controlLabelColor = ViewTemplate::ControlsLabelColor;

		setFont(controlsLabelFont);
		setTextColor(Color(0x50, 0x50, 0x50));
		setBackgroundColor(Color(255, 255, 255));
		setTransparent(false);
	}

}