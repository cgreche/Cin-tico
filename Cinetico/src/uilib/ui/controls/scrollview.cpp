
// ScrollView
// File: scrollview.cpp
// Last edit: 25/10/2017 05:37 (UTC-3)
// Author: CGR

#include "../uibase.h"

namespace uilib {

	void onScroll_(ScrollBar &scrollBar, int oldPos, int newPos) {
		ScrollView *scroll = (ScrollView*)scrollBar.param();
		Layout *contentLayout = scroll->contentLayout;
		if(contentLayout)
			contentLayout->setPosition(Point(contentLayout->position().x(), -newPos));
	}

	ScrollView::ScrollView()
		: Control(*new OSDScrollView(*this))
		, horizontalScrollBar(ScrollBar::Horizontal) {
		contentLayout = &dummyLayout;

		verticalScrollBar.addOnScroll(onScroll_);
		verticalScrollBar.setParam(this);

		hLayout.append(*contentLayout,MaximumSize);
		hLayout.append(verticalScrollBar);
		viewLayout.append(hLayout);
		viewLayout.append(horizontalScrollBar);
		Control::setLayout(&viewLayout);

		osdRef().create();
	}

	ScrollView::~ScrollView() {

	}

	void ScrollView::setLayout(Layout *layout) {
		hLayout.remove(*contentLayout);
		if (!layout)
			contentLayout = &dummyLayout;
		else
			contentLayout = layout;
		hLayout.insertBefore(verticalScrollBar,*contentLayout,Size(SizeTypeAuto,SizeTypeAuto));
		this->setSize(this->size());

		Size contentSize = contentLayout->size();
		Size workingSize = getFrameSize();
		verticalScrollBar.setScrollLength(contentSize.height());
		verticalScrollBar.setPageSize(workingSize.height());
	}

}