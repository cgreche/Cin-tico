
//ComboBox
// File: combobox.h
// Last edit: 01/08/2014 01:35 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __COMBOBOX_H__
#define __COMBOBOX_H__

#include <vector>

namespace uilib {

	class ComboBox;
	class ComboBoxItem;

	typedef void(*ComboFunc)(ComboBox &combo, ComboBoxItem *item);


	class ComboBoxItem
	{
		string m_text;
		u32 state;

		Color m_textcolor;
		Font m_font;

		void *m_param;

	public:
		friend class ComboBox;

		ComboBoxItem(const string &text,
			const Color &textcolor,
			FontDesc &fd,
			void *param = NULL) :
			m_text(text), m_textcolor(textcolor),
			m_param(param)
		{
			m_font.set(fd);
		}

		const string &text() const { return m_text; }
		Color textColor() const { return m_textcolor; }
		void *param() const { return m_param; }

		Font &font() { return m_font; }

	};


	class ComboBox : public Control
	{
		ComboFunc m_onSelect;

		std::vector<ComboBoxItem*> m_items;
		u32 m_selection;

	public:
		ComboBox();

		void setOnSelect(ComboFunc onSelect) { m_onSelect = onSelect; }

		u32 fastinsertItem(const string &text);
		u32 insertItem(ComboBoxItem &item);
		void removeItem(u32 index);
		void removeItem(ComboBoxItem &item);
		void reset();
		void sort();
		u32 setSelection(u32 row);

		u32 selection();
		ComboBoxItem *item();
		ComboBoxItem *item(u32 row);

		virtual Size getAutoSize();


		friend class OSDComboBox;
		OSDComboBox& osdRef() const { return reinterpret_cast<OSDComboBox&>(Control::osdRef()); }
	};

}

#endif