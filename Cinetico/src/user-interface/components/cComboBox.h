#ifndef __CINETICO_COMBOBOX_H__
#define __CINETICO_COMBOBOX_H__

#include "uilib/ui/uibase.h"

namespace cinetico {
	
	using namespace uilib;

	class cComboBox : public VerticalLayout {

	public:
		Label label;
		uilib::ComboBox combo;

		cComboBox();

		void setLabel(const string &text) { label.setText(text); }
		void sort() { combo.sort(); }
		void clear() { combo.reset(); }
		void appendItem(const string &text, int value = 0) { combo.insertItem(*new ComboBoxItem(text, uilib::Color(0,0,0), FontDesc("Arial",10,0),(void*)value)); }
		void appendItem(const string &text, void *data) { combo.insertItem(*new ComboBoxItem(text, uilib::Color(0, 0, 0), FontDesc("Arial", 10, 0), data)); };
		void setSelection(int index) { combo.setSelection(index); }
		void setSelectionByData(void *data);
		int selection() const { return combo.selection(); }
		ComboBoxItem *item(int index) const { return combo.item(index); }
		ComboBoxItem *selectedItem() const { return selection() != -1 ? combo.item(selection()) : NULL; }
	};

}

#endif