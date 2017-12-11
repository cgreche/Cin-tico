
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

	void cComboBox::setSelectionByData(void *data) {
		for (int i = 0; i < combo.itemCount(); ++i) {
			ComboBoxItem *item = combo.item(i);
			if (item->data() == data)
				combo.setSelection(i);
		}
	}

}