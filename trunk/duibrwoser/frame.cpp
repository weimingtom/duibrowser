//
// main_frame.cpp
// ~~~~~~~~~~~~~~
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

#include "stdafx.h"
#include <EAWebKit/EAWebKit.h>
#include <EAWebkit/EAWebkitAllocator.h>
#include <EAWebKit/EAWebKitTextInterface.h>
#include <PPMalloc/EAStackAllocator.h>
#include <stdlib.h>  // malloc / new

#include "win_impl_base.hpp"
#include "UIWebkit.hpp"
#include "frame.hpp"

#if !defined(UNDER_CE) && defined(_DEBUG)
#define new   new(_NORMAL_BLOCK, __FILE__, __LINE__)
#endif

namespace EA {
	namespace TextWrapper {
		class FontServerProxy;
	}
}

using namespace EA::TextWrapper;

const int kViewTickTimerId = 1001;
const int kViewTickTimerElapse = 360;
const TCHAR* const kWebkitControlName = _T("webkit");
const int kDefaultFontSize = 18;
const int kMiniFontSize = 12;
const int kPageTimeoutSeconds = 30;

class MyAllocator : public Allocator
{
public:
	void* Malloc(size_t size, int flags, const char* pName)
	{
		return allocator_.Malloc(size);
	}

	void* MallocAligned(size_t size, size_t alignment, size_t offset, int flags, const char* pName)
	{
		return allocator_.MallocAligned(size, alignment, offset);
	}

	void  Free(void* p, size_t size)
	{
		return allocator_.Free(p);
	}

	void* Realloc(void* p, size_t size, int flags)
	{
		return allocator_.Realloc(p, size);
	}

private:
	StackAllocator allocator_;
};

MainFrame::MainFrame()
: webkit_dll_(NULL)
, webkit_(NULL)
, view_(NULL)
, font_server_(NULL)
, glyph_cache_(NULL)
, surface_(NULL)
, raster_(NULL)
, allocator_(NULL)
, font_style_(NULL)
, did_first_layout_(false)
{
	allocator_ = new MyAllocator();
}

MainFrame::~MainFrame()
{
	delete allocator_;
	allocator_ = NULL;
	PostQuitMessage(0);
}

LPCTSTR MainFrame::GetWindowClassName() const
{
	return _T("DuiWebkitBrowser");
}

CControlUI* MainFrame::CreateControl(LPCTSTR pstrClass, CPaintManagerUI* pManager)
{
	if (_tcsicmp(pstrClass, _T("Webkit")) == 0)
		return new CWebkitUI();

	return NULL;
}

void MainFrame::OnFinalMessage(HWND hWnd)
{
	if ((webkit_ != NULL) && (view_ != NULL))
	{
		view_->CancelLoad();	
		view_->ShutdownView();
		webkit_->DestroyView(view_);
		webkit_->DestroyFontServerWrapperInterface(font_server_);
		webkit_->DestroyGlyphCacheWrapperInterface(glyph_cache_);	
		webkit_->Shutdown();
		webkit_->Destroy();

		if (font_style_)
			font_style_->Destroy();

		view_ = NULL;
		webkit_ = NULL;

		FreeLibrary(webkit_dll_);
		webkit_dll_ = NULL;
	}

	WindowImplBase::OnFinalMessage(hWnd);
	delete this;
}

tString MainFrame::GetSkinFile()
{
	return _T("main.xml");
}

void MainFrame::HandleMouseMoveEvent(MouseMoveEvent mouseEvent)
{
    view_->OnMouseMoveEvent(mouseEvent);
}

void MainFrame::HandleMouseButtonEvent(MouseButtonEvent mouseEvent)
{
    view_->OnMouseButtonEvent(mouseEvent);
}

void MainFrame::HandleMouseWheelEvent(MouseWheelEvent mouseEvent)
{
    view_->OnMouseWheelEvent(mouseEvent);
}

void MainFrame::HandleKeyboardEvent(KeyboardEvent keyEvent)
{	
    view_->OnKeyboardEvent(keyEvent);
}

void MainFrame::HandleFocusChangeEvent(bool bHasFocus)
{
    view_->OnFocusChangeEvent(bHasFocus);
}

CPoint MainFrame::GetClientPoint(LPARAM lParam, bool relative_upper_left_screen)
{
	CPoint point(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
	if (relative_upper_left_screen)
		ScreenToClient(m_hWnd, &point);

	CWebkitUI* webkit_control = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
	if (webkit_control)
	{
		CRect webkit_rect = webkit_control->GetPos();
		point.x -= webkit_rect.left;
		point.y -= webkit_rect.top;
	}

	return point;
}

LRESULT MainFrame::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static CPoint globalPrevPoint;

	if (view_)
	{
		switch (uMsg)
		{
		case WM_MOUSEMOVE:
			if (did_first_layout_)
			{
				CWebkitUI* webkit_control = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
				CRect webkit_rect;
				if (webkit_control)
					webkit_rect = webkit_control->GetPos();
				if (webkit_rect.PtInRect(CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))))
				{
					MouseMoveEvent mouseEvent;

					CPoint point = GetClientPoint(lParam);

					mouseEvent.mX = point.x;
					mouseEvent.mY = point.y;

					mouseEvent.mDX = -(point.x - globalPrevPoint.x);
					mouseEvent.mDY = -(point.y - globalPrevPoint.y);

					mouseEvent.mbShift = wParam & MK_SHIFT;
					mouseEvent.mbControl = wParam & MK_CONTROL;

					HandleMouseMoveEvent(mouseEvent);
				}
			}
			break;

        case WM_LBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_LBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_RBUTTONDBLCLK:
        case WM_LBUTTONUP:
        case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			if (did_first_layout_)
			{
				CWebkitUI* webkit_control = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
				CRect webkit_rect;
				if (webkit_control)
					webkit_rect = webkit_control->GetPos();
				if (webkit_rect.PtInRect(CPoint(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam))))
				{
					CPoint point = GetClientPoint(lParam);
					globalPrevPoint = point;

					MouseButtonEvent mouseEvent;
					mouseEvent.mX = point.x;
					mouseEvent.mY = point.y;

					if (uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK || uMsg == WM_LBUTTONUP)
						mouseEvent.mId = kMouseLeft;
					else if (uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK || uMsg == WM_MBUTTONUP)
						mouseEvent.mId = kMouseMiddle;
					else if (uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK || uMsg == WM_RBUTTONUP)
						mouseEvent.mId = kMouseRight;

					if (uMsg == WM_LBUTTONDOWN || uMsg == WM_MBUTTONDOWN || uMsg == WM_RBUTTONDOWN)
						mouseEvent.mbDepressed = true;
					else if (uMsg == WM_LBUTTONDBLCLK || uMsg == WM_MBUTTONDBLCLK || uMsg == WM_RBUTTONDBLCLK)
						mouseEvent.mbDepressed = false;
					else if (uMsg == WM_LBUTTONUP || uMsg == WM_MBUTTONUP || uMsg == WM_RBUTTONUP)
						mouseEvent.mbDepressed = false;

					mouseEvent.mbShift = wParam & MK_SHIFT;
					mouseEvent.mbControl = wParam & MK_CONTROL;

					HandleMouseButtonEvent(mouseEvent);
				}
			}
            break;

        case WM_MOUSEWHEEL:
			if (did_first_layout_)
			{
				MouseWheelEvent mouseEvent;

				mouseEvent.mX = GetClientPoint(lParam, true).x;
				mouseEvent.mY = GetClientPoint(lParam, true).y;

				short fwKeys = GET_KEYSTATE_WPARAM(wParam);
				short zDelta = GET_WHEEL_DELTA_WPARAM(wParam);

				mouseEvent.mZDelta = zDelta;

				// default value is 3
				UINT param = 3;
				SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &param, 0);
				mouseEvent.mLineDelta = param;

				mouseEvent.mbShift = fwKeys & MK_SHIFT;
				mouseEvent.mbControl = fwKeys & MK_CONTROL;

				// Ctrl+Mouse wheel doesn't ever go into WebCore.  It is used to
				// zoom instead (Mac zooms the whole Desktop, but Windows browsers trigger their
				// own local zoom modes for Ctrl+wheel).
				//if (wParam & MK_CONTROL)
				//{
				//	short delta = short(HIWORD(wParam));
				//	if (delta < 0)
				//		makeTextLarger();
				//	else
				//		makeTextSmaller();
				//}

				HandleMouseWheelEvent(mouseEvent);
			}
            break;

        case WM_SYSKEYDOWN:
        case WM_KEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYUP:
        case WM_SYSCHAR:
        case WM_CHAR:
			if (did_first_layout_)
			{
				KeyboardEvent keyEvent;

				keyEvent.mId = wParam;
				keyEvent.mbChar = false;
				keyEvent.mbDepressed = false;

				if (uMsg == WM_CHAR)
					keyEvent.mbChar = true;

				if (uMsg == WM_KEYDOWN || uMsg == WM_SYSKEYDOWN)
					keyEvent.mbDepressed = true;

				HandleKeyboardEvent(keyEvent);
			}
            break;

		case WM_SETFOCUS:
		case WM_KILLFOCUS:
			HandleFocusChangeEvent(uMsg == WM_SETFOCUS);
			break;
		}
	}

	return __super::HandleMessage(uMsg, wParam, lParam);
}

LRESULT MainFrame::ResponseDefaultKeyEvent(WPARAM wParam)
{
	if (wParam == VK_RETURN)
	{
		return FALSE;
	}
	else if (wParam == VK_ESCAPE)
	{
		return TRUE;
	}
	return FALSE;
}

void MainFrame::OnTimer(TNotifyUI& msg)
{
	switch (msg.wParam)
	{
	case kViewTickTimerId:
		if (view_ != NULL) view_->Tick();
		//paint_manager_.KillTimer(msg.pSender, kViewTickTimerId);
		break;
	default:
		break;
	}
}

LRESULT MainFrame::MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return __super::MessageHandler(uMsg, wParam, lParam, bHandled);
}

void MainFrame::OnExit(TNotifyUI& msg)
{
	Close();
}

void MainFrame::Init()
{
	typedef EA::WebKit::IEAWebkit* (* PF_CreateEAWebkitInstance)(void);
	PF_CreateEAWebkitInstance create_webkit_instance = NULL;

	webkit_dll_ = LoadLibrary(_T("EAWebkit.dll"));
	if (webkit_dll_ != NULL)
#if defined(UI_BUILD_FOR_WINCE)
		create_webkit_instance = reinterpret_cast<PF_CreateEAWebkitInstance>(GetProcAddress(webkit_dll_, _T("CreateEAWebkitInstance")));
#else
		create_webkit_instance = reinterpret_cast<PF_CreateEAWebkitInstance>(GetProcAddress(webkit_dll_, "CreateEAWebkitInstance"));
#endif

	CWebkitUI* webkit_control = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
	if ((create_webkit_instance != NULL) && (webkit_control != NULL))
	{
		webkit_ = create_webkit_instance();
		webkit_->Init(NULL);

		glyph_cache_ = webkit_->CreateGlyphCacheWrapperInterface(NULL);
		font_server_ = webkit_->CreateFontServerWrapperInterface(NULL);

		font_style_ = font_server_->CreateTextStyle();
		font_style_->SetSize(14);
		font_style_->SetSmooth(kSmoothEnabled);
		font_style_->SetWeight(1200);

		wchar_t szWindowsDir[MAX_PATH] = {0};
		GetWindowsDirectoryW(szWindowsDir, MAX_PATH);
		wchar_t szFontDir[MAX_PATH] = {0};
#if defined(UI_BUILD_FOR_WINCE)
		swprintf(szFontDir, L"%s\\Fonts", szWindowsDir);
#else
		swprintf_s(szFontDir, MAX_PATH - 1, L"%s\\Fonts", szWindowsDir);
#endif
		font_server_->AddDirectory(szFontDir, L"*.*");
		//font_server_->AddDirectory(L"./", L"*.ttf");

		Parameters& param = webkit_->GetParameters();

		// default "en-us"
		param.mpLocale = "zh-cn";

		param.mDefaultFontSize = kDefaultFontSize;
		param.mDefaultMonospaceFontSize = kDefaultFontSize;
		param.mMinimumFontSize = kMiniFontSize;
		param.mMinimumLogicalFontSize = kMiniFontSize;

		param.mEnableSmoothText = true;
		param.mFontSmoothingEnabled = true;

		param.mSystemFontDescription.mSize = kDefaultFontSize;
		sprintf_s(param.mSystemFontDescription.mFamilies, sizeof(param.mSystemFontDescription.mFamilies) / sizeof(param.mSystemFontDescription.mFamilies[0]),\
			"System");
#if 1
		sprintf_s(param.mFontFamilyStandard, sizeof(param.mFontFamilyStandard) / sizeof(param.mFontFamilyStandard[0]), "System");
		sprintf_s(param.mFontFamilySerif, sizeof(param.mFontFamilySerif) / sizeof(param.mFontFamilySerif[0]), "Times New Roman");
		sprintf_s(param.mFontFamilySansSerif, sizeof(param.mFontFamilySansSerif) / sizeof(param.mFontFamilySansSerif[0]), "Tahoma");
		sprintf_s(param.mFontFamilyMonospace, sizeof(param.mFontFamilyMonospace) / sizeof(param.mFontFamilyMonospace[0]), "System");
		sprintf_s(param.mFontFamilyCursive, sizeof(param.mFontFamilyCursive) / sizeof(param.mFontFamilyCursive[0]), "Times New Roman");
		sprintf_s(param.mFontFamilyFantasy, sizeof(param.mFontFamilyFantasy) / sizeof(param.mFontFamilyFantasy[0]), "System");
#else
		//iFonts.default_font = _strdup("Bitstream Vera Sans");
		//iFonts.cursive_font = _strdup("Times New Roman");
		//iFonts.fantasy_font = _strdup("System");
		//iFonts.monospace_font = _strdup("Bitstream Vera Sans Mono");
		//iFonts.sans_serif_font = _strdup("Tahoma");
		//iFonts.serif_font = _strdup("Bitstream Vera Serif");
#endif

		view_ = webkit_->CreateView();
		webkit_->SetViewNotification(this);
		raster_ = webkit_->GetEARasterInstance();
	}
}

void MainFrame::OnPrepare(TNotifyUI& msg)
{
	CControlUI* title = paint_manager_.FindControl(_T("header"));
	if (title != NULL)
		paint_manager_.SetTimer(title, kViewTickTimerId, kViewTickTimerElapse);

	CWebkitUI* webkit_control = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
	if (webkit_control != NULL)
	{
		ViewParameters vp = view_->GetParameters();
		vp.mbTransparentBackground = false;
		vp.mWidth = webkit_control->GetPos().right - webkit_control->GetPos().left;
		vp.mWidth = (vp.mWidth + 3) / 4 * 4;
		vp.mHeight = webkit_control->GetPos().bottom - webkit_control->GetPos().top;
		view_->InitView(vp);

		webkit_control->SetEARasterAndView(raster_, view_);

		//view_->SetURI("file:///E:/Webkit/chapter02/chapter02.html");
		view_->SetURI("http://www.oschina.net/");
	}
}

void MainFrame::Notify(TNotifyUI& msg)
{
	if (_tcsicmp(msg.sType, DuiLib::kWindowInit) == 0)
	{
		OnPrepare(msg);
	}
	else if (_tcsicmp(msg.sType, DuiLib::kClick) == 0)
	{
		int view_count = webkit_->GetViewCount();
		view_ = webkit_->GetView(0);

		if (_tcsicmp(msg.pSender->GetName(), _T("closebtn")) == 0)
			OnExit(msg);
		else if (_tcsicmp(msg.pSender->GetName(), _T("minbtn")) == 0)
		{
#if defined(UI_BUILD_FOR_WINCE)
			::ShowWindow(m_hWnd, SW_MINIMIZE);
#else
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0);
#endif
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("backbtn")) == 0)
		{
			if (view_ != NULL)
				view_->GoBack();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("forwardbtn")) == 0)
		{
			if (view_ != NULL)
				view_->GoForward();
		}
		else if (_tcsicmp(msg.pSender->GetName(), _T("refreshbtn")) == 0)
		{
			if (view_ != NULL)
				view_->Refresh();
		}
	}
	else if (_tcsicmp(msg.sType, DuiLib::kTimer) == 0)
	{
		return OnTimer(msg);
	}
}

bool MainFrame::ViewUpdate(ViewUpdateInfo& view_update_info)
{
	did_first_layout_ = true;
	CWebkitUI* webkit_control = static_cast<CWebkitUI*>(paint_manager_.FindControl(kWebkitControlName));
	if (webkit_control != NULL)
		webkit_control->LayoutChanged();
	return true;
}

bool MainFrame::LoadUpdate(LoadInfo& load_info)
{
	CControlUI* app_title = paint_manager_.FindControl(_T("apptitle"));
	if ((app_title != NULL) && load_info.mbCompleted && _tcslen(webkit_->GetCharacters(load_info.mPageTitle)) > 0)
	{
		TCHAR szTitle[MAX_PATH] = {0};
		_stprintf(szTitle, webkit_->GetCharacters(load_info.mPageTitle));
		app_title->SetText(webkit_->GetCharacters(load_info.mPageTitle));
	}

	return true;
}