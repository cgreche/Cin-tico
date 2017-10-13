
#include "cTextBox.h"
#include "../viewtemplate.h"


namespace cinetico {
	
	cTextBox::cTextBox() {
		FontDesc &controlsLabelFont = ViewTemplate::ControlsLabelFont;
		Color &controlLabelColor = ViewTemplate::ControlsLabelColor;

		label.setFont(controlsLabelFont);
		label.setTextColor(controlLabelColor);

		append(label);
		append(edit);
	}

}