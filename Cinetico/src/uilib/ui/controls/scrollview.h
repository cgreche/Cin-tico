
// ScrollView
// File: scrollview.h
// Last edit: 25/10/2017 05:37 (UTC-3)
// Author: CGR

#ifndef __UILIB_SCROLLVIEW_H__
#define __UILIB_SCROLLVIEW_H__

#include "control.h"
#include "scrollbar.h"

namespace uilib {

	class ScrollView : public Control
	{
		VerticalLayout dummyLayout;
	protected:
		friend class OSDScrollView;
		OSDScrollView &osdRef() const { return reinterpret_cast<OSDScrollView&>(Control::osdRef()); }

		VerticalLayout viewLayout;
			HorizontalLayout hLayout;
				Layout *contentLayout;
				VerticalScrollBar verticalScrollBar;
			HorizontalScrollBar horizontalScrollBar;
		int m_workArea;


	public:
		ScrollView();
		~ScrollView();
		virtual void setLayout(Layout *layout);
	};

}

#endif