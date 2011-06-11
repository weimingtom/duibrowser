//
// UIWebkit.cpp
// ~~~~~~~~~~~~
//
// Copyright (c) 2011 achellies (achellies at 163 dot com)
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//
#include <EAWebKit/EAWebKit.h>
#include <EAWebKit/EAWebkitSTLWrapper.h>
#include "UIlib.h"
#include "UIWebkit.hpp"

namespace DuiLib {
	
CWebkitUI::CWebkitUI()
: bitmap_bits_(NULL)
, did_first_layout_(false)
{
    last_mouse_point_.x = 0;
    last_mouse_point_.y = 0;
}

CWebkitUI::~CWebkitUI()
{
	if (bitmap_bits_ != NULL)
		delete[] bitmap_bits_;
}

bool CWebkitUI::LayoutChanged()
{
	did_first_layout_ = true;
	Invalidate();

	return did_first_layout_;
}

void CWebkitUI::SetEARasterAndView(IEARaster* raster, View* view)
{
	raster_ = raster;
	view_ = view;
}

void CWebkitUI::DoEvent(TEventUI& event)
{
    if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
        if( m_pParent != NULL ) m_pParent->DoEvent(event);
        else CControlUI::DoEvent(event);
        return;
    }

    if( event.Type == UIEVENT_SETFOCUS ) 
    {
        view_->OnFocusChangeEvent(true);
    }
    if( event.Type == UIEVENT_KILLFOCUS ) 
    {
        view_->OnFocusChangeEvent(false);
    }
    if( event.Type == UIEVENT_CHAR )
    {
        KeyboardEvent keyEvent = {0};
        keyEvent.mId = event.chKey;
        keyEvent.mbChar = true;
        keyEvent.mbDepressed = false;
        keyEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        keyEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        keyEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnKeyboardEvent(keyEvent);
    }
    if( event.Type == UIEVENT_KEYDOWN )
    {
        KeyboardEvent keyEvent = {0};
        keyEvent.mId = event.chKey;
        keyEvent.mbChar = false;
        keyEvent.mbDepressed = true;
        keyEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        keyEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        keyEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnKeyboardEvent(keyEvent);
    }
    if( event.Type == UIEVENT_KEYUP )
    {
        KeyboardEvent keyEvent = {0};
        keyEvent.mId = event.chKey;
        keyEvent.mbChar = false;
        keyEvent.mbDepressed = false;
        keyEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        keyEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        keyEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnKeyboardEvent(keyEvent);
    }
    if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK || event.Type == UIEVENT_BUTTONUP )
    {
        MouseButtonEvent mouseEvent = {0};
        mouseEvent.mX = event.ptMouse.x - m_rcItem.left;
        mouseEvent.mY = event.ptMouse.y - m_rcItem.top;
        mouseEvent.mId = kMouseLeft;
        if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
            mouseEvent.mbDepressed = true;
        mouseEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        mouseEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        mouseEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnMouseButtonEvent(mouseEvent);
        return;
    }
    if( event.Type == UIEVENT_MOUSEMOVE )
    {
        MouseMoveEvent mouseEvent = {0};
        mouseEvent.mX = event.ptMouse.x - m_rcItem.left;
        mouseEvent.mY = event.ptMouse.y - m_rcItem.top;
        mouseEvent.mDX = event.ptMouse.x - last_mouse_point_.x;
        mouseEvent.mDY = event.ptMouse.y - last_mouse_point_.y;
        mouseEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        mouseEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        mouseEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnMouseMoveEvent(mouseEvent);
        last_mouse_point_.x = mouseEvent.mX;
        last_mouse_point_.y = mouseEvent.mY;
        return;
    }
    if( event.Type == UIEVENT_SCROLLWHEEL )
    {
		MouseWheelEvent mouseEvent = {0};
		mouseEvent.mX = event.ptMouse.x - m_rcItem.left;
		mouseEvent.mY = event.ptMouse.y - m_rcItem.top;

		// default value is 3
		int uLineDelta = 3;
		SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &uLineDelta, 0);

        if( LOWORD(event.wParam) == SB_LINEUP ) mouseEvent.mLineDelta = uLineDelta;
        else mouseEvent.mLineDelta = -uLineDelta;
        mouseEvent.mbShift = ((event.wKeyState & MK_SHIFT) != 0);
        mouseEvent.mbControl = ((event.wKeyState & MK_CONTROL) != 0);
        mouseEvent.mbAlt = ((event.wKeyState & MK_ALT) != 0);
        view_->OnMouseWheelEvent(mouseEvent);
        return;
    }
}

void CWebkitUI::DoPaint(void* ctx, const RECT& rcPaint)
{
	if ((ctx == NULL) || (raster_ == NULL))
		return;

    if( !::IntersectRect(&m_rcPaint, &rcPaint, &m_rcItem) ) return;

	HDC hDC = (HDC)ctx;

	BITMAPINFO bi = {0};
	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	bi.bmiHeader.biWidth		= ((m_rcItem.right - m_rcItem.left) + 3) / 4 * 4;
	bi.bmiHeader.biHeight		= (m_rcItem.bottom - m_rcItem.top);
	bi.bmiHeader.biPlanes		= 1;
	bi.bmiHeader.biBitCount		= 32;
	bi.bmiHeader.biCompression	= 0;
	bi.bmiHeader.biSizeImage	= bi.bmiHeader.biWidth * bi.bmiHeader.biHeight * 4;
	bi.bmiHeader.biXPelsPerMeter= 0;
	bi.bmiHeader.biYPelsPerMeter= 0;
	bi.bmiHeader.biClrUsed		= 0;
	bi.bmiHeader.biClrImportant	= 0;	

	if (bitmap_bits_ == NULL)
	{
		bitmap_bits_ = new BYTE[bi.bmiHeader.biSizeImage];
		memset(bitmap_bits_, 0xED, bi.bmiHeader.biSizeImage);
	}
	else if (did_first_layout_ && bitmap_bits_ != NULL)
	{
		memcpy_s(bitmap_bits_, bi.bmiHeader.biSizeImage, view_->GetSurface()->mpData, bi.bmiHeader.biSizeImage);
	}
	else if (bitmap_bits_ != NULL)
	{
		memset(bitmap_bits_, 0xED, bi.bmiHeader.biSizeImage);
	}

	::SetStretchBltMode(hDC, COLORONCOLOR);

	BITMAPINFOHEADER bmiHeader = bi.bmiHeader;
	bmiHeader.biHeight = -bmiHeader.biHeight;
	StretchDIBits(hDC, m_rcItem.left, m_rcItem.top, m_rcItem.right - m_rcItem.left, m_rcItem.bottom - m_rcItem.top, 
		0, 0, bi.bmiHeader.biWidth, bi.bmiHeader.biHeight, bitmap_bits_, (LPBITMAPINFO)&bmiHeader, DIB_RGB_COLORS, SRCCOPY);
}

} // namespace DuiLib