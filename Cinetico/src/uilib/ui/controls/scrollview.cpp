
// ScrollView
// File: scrollview.cpp
// Last edit: 25/10/2017 05:37 (UTC-3)
// Author: CGR

#include "../uibase.h"

namespace uilib {

	void onScroll_(VerticalScrollBar &scrollBar) {
		int a = 1;
		RECT rc;
		rc.left = 0;
		rc.top = 0;
		rc.right = 200;
		rc.bottom = 200;

		RECT rc2;
		::GetWindowRect(scrollBar.osdRef().handle(), &rc2);
		::ScrollWindow(scrollBar.parent()->osdRef().handle(), 0, -1, &rc,NULL);
		::GetWindowRect(scrollBar.osdRef().handle(), &rc2);
		::UpdateWindow(scrollBar.parent()->osdRef().handle());

	}

	ScrollView::ScrollView()
		: Control(*new OSDScrollView(*this)) {
		contentLayout = &dummyLayout;

		verticalScrollBar.setOnScroll(onScroll_);

		hLayout.append(*contentLayout,MaximumSize);
		hLayout.append(verticalScrollBar,Size(20,SizeTypeMax));
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
		hLayout.insertBefore(verticalScrollBar,*contentLayout,MaximumSize);
		this->setSize(this->size());
	}

}