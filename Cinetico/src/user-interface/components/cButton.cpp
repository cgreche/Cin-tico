
#include "cButton.h"
#include "../viewtemplate.h"


namespace cinetico {
	
	void cButton::onMouseEnterEvent(MouseEvent &event) {
		Color backgroundColor(0x21, 0x21, 0x21);
		setBackgroundColor(Color(0x21*3.f, 0x21 * 3.f, 0x21 * 3.f));
		//setTextColor(Color(255, 255, 255));
	}

	void cButton::onMouseLeaveEvent(MouseEvent &event) {
		Color backgroundColor(0x21, 0x21, 0x21);
		setBackgroundColor(backgroundColor);
		setTextColor(ViewTemplate::AppTitleColor);
		m_parentLayout->update();
	}

	void cButton::onMousePressEvent(MouseEvent &event) {
		Color backgroundColor(0x21, 0x21, 0x21);
		setBackgroundColor(Color(0x21 * 1.4f, 0x21 * 1.4f, 0x21 * 1.4f));
		setTextColor(ViewTemplate::AppTitleColor);
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
		setTextColor(Color(0x21, 0x21, 0x21));
		setBackgroundColor(ViewTemplate::AppTitleColor);
		setTransparent(false);
		setStyle(style() | CS_Border);
		setAlignment(Label::HCenter | Label::VCenter);
	}

	Size cButton::getAutoSize() {
		Size size = UITools::getTextSize(m_text, m_font.desc());
		size.setWidth(size.width() + 20);
		size.setHeight(size.height() + 10);
		return size;
	}

}