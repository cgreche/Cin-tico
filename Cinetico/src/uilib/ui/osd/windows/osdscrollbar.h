//OSD ScrollView
// File: osdscrollview.h
// Last edit: 18/12/2017 04:32 (UTC-3)
// Author: CGR

#include "osdbase.h"

namespace uilib {
	
	class OSDScrollBar : public OSDControl
	{
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		friend class ScrollBar;
		OSDScrollBar(ScrollBar &ref) : OSDControl(ref) { }
		ScrollBar& ref() const { return static_cast<ScrollBar&>(OSDControl::ref()); }

		void setScrollLength(int length);
		void setPageSize(int pageSize);
		void setScrollPosition(int pos);

		//OSDUIObj methods
		virtual void create();
		virtual void update();

		//OSDControl methods
		virtual Size getAutoSize();
	};

}