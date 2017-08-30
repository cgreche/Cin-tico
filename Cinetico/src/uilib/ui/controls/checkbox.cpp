
//Check Box
// File name: checkbox.cpp
// Last edit: 03/01/2014 13:20 (UTC-3)
// Author: CGR

#include "..\\uibase.h"



CheckBox::CheckBox():Button(*new OSDCheckBox(*this))
{
	m_checked = false;
	osdRef().create();
	setDefaultFont();
}


Size CheckBox::getAutoSize()
{
	return osdRef().getAutoSize();
}


void CheckBox::setChecked(bool checked)
{
	m_checked = checked;
	osdRef().setChecked(checked);
}

bool CheckBox::checked() const
{
	return m_checked;
}
