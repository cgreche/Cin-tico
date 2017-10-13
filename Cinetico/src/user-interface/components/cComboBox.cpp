
#include "cComboBox.h"
#include "../viewtemplate.h"

namespace cinetico {

	cComboBox::cComboBox() {
		FontDesc &controlsLabelFont = ViewTemplate::ControlsLabelFont;
		Color &controlLabelColor = ViewTemplate::ControlsLabelColor;

		label.setFont(controlsLabelFont);
		label.setTextColor(controlLabelColor);

		append(label);
		append(combo, Size(SizeTypeMax, SizeTypeAuto));
	}

}