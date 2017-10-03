
//OSD Scrollbar
// File: osdscrollbar.cpp
// Last edit: 04/08/2014 23:37 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"
#include <math.h>

namespace uilib {

	void OSDVerticalScrollBar::create()
	{
		createWindow(*this, WC_SCROLLBAR, 0, WS_TABSTOP | SBS_VERT);
	}

	void OSDVerticalScrollBar::update()
	{
		OSDControl::update();
		setScrollSize(ref().m_workingSize, ref().m_contentSize);
		setScrollPosition(ref().scrollPosition());
	}

	void OSDVerticalScrollBar::setScrollSize(const Size &workingSize, const Size &contentSize)
	{
		SCROLLINFO scrollinfo;
		scrollinfo.cbSize = sizeof(SCROLLINFO);
		scrollinfo.fMask = SIF_RANGE | SIF_PAGE;
		scrollinfo.nMin = 0;
		scrollinfo.nMax = contentSize.height() - 1;
		scrollinfo.nPage = workingSize.height();
		SetScrollInfo(m_hwnd, SB_CTL, &scrollinfo, TRUE);

		scrollinfo.fMask = SIF_POS;
		GetScrollInfo(m_hwnd, SB_CTL, &scrollinfo);
		this->ref().m_scrollPos = scrollinfo.nPos;
	}


	void OSDVerticalScrollBar::setScrollPosition(u32 pos)
	{
		SCROLLINFO scrollinfo;
		scrollinfo.cbSize = sizeof(SCROLLINFO);
		scrollinfo.fMask = SIF_POS;
		scrollinfo.nPos = pos;
		SetScrollInfo(m_hwnd, SB_CTL, &scrollinfo, TRUE);
		GetScrollInfo(m_hwnd, SB_CTL, &scrollinfo);
		this->ref().m_scrollPos = scrollinfo.nPos;
	}

	Size OSDVerticalScrollBar::getAutoSize()
	{
		int w = ::GetSystemMetrics(SM_CXVSCROLL);
		return Size(w, SizeTypeAuto);
	}

	LRESULT OSDVerticalScrollBar::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_VSCROLL) {
			//windows may have clamp'd the position
			SCROLLINFO scrollinfo;
			scrollinfo.cbSize = sizeof(SCROLLINFO);
			scrollinfo.fMask = SIF_POS;
			GetScrollInfo((HWND)lParam, SB_CTL, &scrollinfo);
			this->ref().m_scrollPos = scrollinfo.nPos;

			if (this->ref().m_onScroll)
				this->ref().m_onScroll(this->ref());

			/*
			Return value

			If an application processes this message, it should return zero.
			*/

			return 0;
		}
		return ::DefWindowProc(ref().parent()->osdRef().handle(), msg, wParam, lParam);
	}




	void OSDHorizontalScrollBar::create()
	{
		createWindow(*this, WC_SCROLLBAR, 0, WS_TABSTOP | SBS_HORZ);
	}

	void OSDHorizontalScrollBar::update()
	{
		OSDControl::update();
		setScrollSize(ref().m_workingSize, ref().m_contentSize);
		setScrollPosition(ref().scrollPosition());
	}

	Size OSDHorizontalScrollBar::getAutoSize()
	{
		int h = ::GetSystemMetrics(SM_CYHSCROLL);
		return Size(SizeTypeAuto, h);
	}

	void OSDHorizontalScrollBar::setScrollSize(const Size &workingSize, const Size &contentSize)
	{
		//Which are the best values?
		//Content size = the real size
		//Working size (page size) = the visible at a time size
		//Example: A window's height is 300 pixels, an image height is 900.
		//Then, content size will be 300, working size will be 900, which will lead 2 pages to see the image entirely
		//Another way to think is: Working (page) size is the number of elements you can see at a time (pixels in the above case),
		//and the Content size is the number of elements.

		SCROLLINFO scrollinfo;
		scrollinfo.cbSize = sizeof(SCROLLINFO);
		scrollinfo.fMask = SIF_RANGE | SIF_PAGE;
		scrollinfo.nMin = 0;
		scrollinfo.nMax = contentSize.width() > 0 ? contentSize.width() - 1 : 0;
		scrollinfo.nPage = workingSize.width();

		SetScrollInfo(m_hwnd, SB_CTL, &scrollinfo, TRUE);

		scrollinfo.fMask = SIF_POS | SIF_RANGE;
		GetScrollInfo(m_hwnd, SB_CTL, &scrollinfo);
		this->ref().m_scrollPos = scrollinfo.nPos;
	}

	void OSDHorizontalScrollBar::setScrollPosition(u32 pos)
	{
		SCROLLINFO scrollinfo;
		scrollinfo.cbSize = sizeof(SCROLLINFO);
		scrollinfo.fMask = SIF_POS;
		scrollinfo.nPos = pos;
		SetScrollInfo(m_hwnd, SB_CTL, &scrollinfo, TRUE);
		GetScrollInfo(m_hwnd, SB_CTL, &scrollinfo);
		this->ref().m_scrollPos = scrollinfo.nPos;

	}

	/*
	LRESULT OSDWindow::OnHScroll(WPARAM wParam, LPARAM lParam)
	{
		//Return Value
		//If an application processes this message, it should return zero.

		HWND hwnd;
		SCROLLINFO si;
		int curPos;

		//check if this message is from a control
		if(lParam != NULL) {
			UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam,GWLP_USERDATA));
			if(uiobj) {
				OSDUIObj& osdUIObj = uiobj->osdRef();
				return osdUIObj.HandleParentMessage(WM_HSCROLL,wParam,lParam);
			}
			return 0;
		}

		//this is a window scrollbar
		if(LOWORD(wParam) == SB_ENDSCROLL)
			return 0;

		hwnd = m_hwnd;

		si.cbSize = sizeof (si);
		si.fMask  = SIF_ALL;
		::GetScrollInfo (hwnd, SB_HORZ, &si);

		// Save the position for comparison later on
		curPos = si.nPos;
		switch(LOWORD(wParam))
		{
			case SB_LINELEFT: si.nPos--; break; // user clicked the top arrow
			case SB_LINERIGHT: si.nPos++; break; // user clicked the bottom arrow
			case SB_PAGELEFT: si.nPos -= si.nPage; break; // user clicked the scroll bar shaft above the scroll box
			case SB_PAGERIGHT: si.nPos += si.nPage; break;  // user clicked the scroll bar shaft below the scroll box
			case SB_THUMBTRACK:	si.nPos = si.nTrackPos; break; // user dragged the scroll box
			case SB_LEFT: si.nPos = si.nMin; break; // user clicked the HOME keyboard key
			case SB_RIGHT: si.nPos = si.nMax; break; // user clicked the END keyboard key
		}

		//Set the position and then retrieve it. Due to adjustments
		//by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		::SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		::GetScrollInfo(hwnd, SB_HORZ, &si);

		if(curPos != si.nPos) {
			ref().m_hScrollPos = si.nPos;
			if(ref().m_onHScroll)
				ref().m_onHScroll(ref());
		}

		return 0;
	}
	*/

	LRESULT OSDHorizontalScrollBar::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_HSCROLL) {

			//this is a window scrollbar
			if (LOWORD(wParam) == SB_ENDSCROLL)
				return 0;

			HWND hwnd = (HWND)lParam;

			SCROLLINFO si;
			int curPos;

			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_ALL;
			::GetScrollInfo(hwnd, SB_CTL, &si);

			// Save the position for comparison later on
			curPos = si.nPos;
			switch (LOWORD(wParam))
			{
			case SB_LINELEFT: si.nPos--; break; // user clicked the top arrow
			case SB_LINERIGHT: si.nPos++; break; // user clicked the bottom arrow
			case SB_PAGELEFT: si.nPos -= si.nPage; break; // user clicked the scroll bar shaft above the scroll box
			case SB_PAGERIGHT: si.nPos += si.nPage; break;  // user clicked the scroll bar shaft below the scroll box 
			case SB_THUMBTRACK:	si.nPos = si.nTrackPos; break; // user dragged the scroll box
			case SB_LEFT: si.nPos = si.nMin; break; // user clicked the HOME keyboard key
			case SB_RIGHT: si.nPos = si.nMax; break; // user clicked the END keyboard key
			}

			//Set the position and then retrieve it. Due to adjustments
			//by Windows it may not be the same as the value set.
			si.fMask = SIF_POS;
			int res = ::SetScrollInfo(hwnd, SB_CTL, &si, TRUE);
			::GetScrollInfo(hwnd, SB_CTL, &si);

			if (curPos != si.nPos) {
				ref().m_scrollPos = si.nPos;
				if (ref().m_onScroll)
					ref().m_onScroll(ref());
			}

			/*
			Return value

			If an application processes this message, it should return zero.
			*/

			return TRUE;
		}
		return ::DefWindowProc(ref().parent()->osdRef().handle(), msg, wParam, lParam);
	}

}