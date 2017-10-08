#ifndef __CINETICO_COMBOBOX_H__
#define __CINETICO_COMBOBOX_H__

#include "uilib/ui/uibase.h"

namespace cinetico {
	
	using namespace uilib;

	class cComboBox : public VerticalLayout {

	public:
		Label label;
		uilib::ComboBox combo;

		cComboBox() {
			append(label);
			append(combo,Size(SizeTypeMax,SizeTypeAuto));
		}

		void setLabel(const char *text) { label.setText(text); }
		void sort() { combo.sort(); }
		void clear() { combo.reset(); }
		void appendItem(const char *text, int value = 0) { combo.insertItem(*new ComboBoxItem(text, Color(0,0,0), FontDesc("Arial",10,0),(void*)value)); }
		void appendItem(const char *text, void *data) { combo.insertItem(*new ComboBoxItem(text, Color(0, 0, 0), FontDesc("Arial", 10, 0), data)); };
		void setSelection(int index) { combo.setSelection(index); }
		int selection() const { return combo.selection(); }
		ComboBoxItem *item(int index) const { return combo.item(index); }

	};

}

#endif