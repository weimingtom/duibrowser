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
{}

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