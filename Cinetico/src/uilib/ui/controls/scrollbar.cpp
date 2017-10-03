
// Scrollbar
// File: scrollbar.cpp
// Last Edit: 26/07/2014 22:14 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	VerticalScrollBar::VerticalScrollBar() : Control(*new OSDVerticalScrollBar(*this))
	{
		m_length = 0;
		m_scrollPos = 0;
		m_onScroll = NULL;

		//
		osdRef().create();
	}

	void VerticalScrollBar::setScrollSize(const Size &workingSize, const Size &contentSize)
	{
		m_length = contentSize.height();
		osdRef().setScrollSize(workingSize, contentSize);
	}

	void VerticalScrollBar::setScrollPosition(u32 pos)
	{
		m_scrollPos = pos;
		osdRef().setScrollPosition(pos);
	}

	void VerticalScrollBar::setOnScroll(VScrollFunc onScroll)
	{
		m_onScroll = onScroll;
	}

	u32 VerticalScrollBar::scrollPosition() const
	{
		return m_scrollPos;
	}

	u32 VerticalScrollBar::length() const
	{
		return m_length;
	}

	//virtual
	Size VerticalScrollBar::getAutoSize()
	{
		return osdRef().getAutoSize();
	}



	HorizontalScrollBar::HorizontalScrollBar() : Control(*new OSDHorizontalScrollBar(*this))
	{
		m_length = 0;
		m_scrollPos = 0;
		m_onScroll = NULL;

		//
		osdRef().create();
	}

	void HorizontalScrollBar::setScrollSize(const Size &workingSize, const Size &contentSize)
	{
		m_length = contentSize.width();
		osdRef().setScrollSize(workingSize, contentSize);
	}

	void HorizontalScrollBar::setScrollPosition(u32 pos)
	{
		m_scrollPos = pos;
		osdRef().setScrollPosition(pos);
	}


	void HorizontalScrollBar::setOnScroll(HScrollFunc onScroll)
	{
		m_onScroll = onScroll;
	}

	u32 HorizontalScrollBar::scrollPosition() const
	{
		return m_scrollPos;
	}

	u32 HorizontalScrollBar::length() const
	{
		return m_length;
	}

	//virtual
	Size HorizontalScrollBar::getAutoSize()
	{
		return osdRef().getAutoSize();
	}

}
