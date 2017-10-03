
// Scrollbar
// File: scrollbar.h
// Last Edit: 01/08/2014 01:39 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

namespace uilib {

	class VerticalScrollBar;
	class HorizontalScrollBar;

	typedef void(*VScrollFunc)(VerticalScrollBar &scrollbar);
	typedef void(*HScrollFunc)(HorizontalScrollBar &scrollbar);



	class VerticalScrollBar : public Control
	{
		Size m_workingSize;
		Size m_contentSize;
		u32 m_length;
		u32 m_scrollPos;
		VScrollFunc m_onScroll;

	public:
		VerticalScrollBar();

		//virtual overrides
		virtual Size getAutoSize();

		//attributes
		void setScrollSize(const Size &workingSize, const Size &contentSize);
		void setScrollPosition(u32 pos);

		//events
		void setOnScroll(VScrollFunc onScroll);

		//getters
		u32 scrollPosition() const;
		u32 length() const;

		friend class OSDVerticalScrollBar;
		OSDVerticalScrollBar& osdRef() const { return reinterpret_cast<OSDVerticalScrollBar&>(Control::osdRef()); }
	};



	class HorizontalScrollBar : public Control
	{
		Size m_workingSize;
		Size m_contentSize;
		u32 m_length;
		u32 m_scrollPos;
		HScrollFunc m_onScroll;

	public:
		HorizontalScrollBar();

		//virtual overrides
		virtual Size getAutoSize();

		//attributes
		void setScrollSize(const Size &workingSize, const Size &contentSize);
		void setScrollPosition(u32 pos);

		//events
		void setOnScroll(HScrollFunc onChange);

		//getters
		u32 scrollPosition() const;
		u32 length() const;

		friend class OSDHorizontalScrollBar;
		OSDHorizontalScrollBar& osdRef() const { return reinterpret_cast<OSDHorizontalScrollBar&>(Control::osdRef()); }
	};

}

#endif