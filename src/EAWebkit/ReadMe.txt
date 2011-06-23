2011年6月23日改动
***********************************************************************
************1. E:\Webkit\Codes\DuiBrowser\src\EAWebkit\Webkit-owb\WebKit\OrigynWebBrowser\WebCoreSupport\WebFrameLoaderClient.cpp
***********************************************************************
Line 421：
修正ViewNotification接收不到网页标题的bug
增加以下代码：
    EA::WebKit::View* pView = EA::WebKit::GetView(m_webFrame->webView());

    if(pView)
    {
        EA::WebKit::LoadInfo& loadInfo = pView->GetLoadInfo();
        loadInfo.mpView = pView;
        loadInfo.mLET   = EA::WebKit::kLETTitleReceived;

		WebCore::KURL kurl(title);
		if(kurl.string().length() != 0)
		{			
			GetFixedString(loadInfo.mPageTitle)->assign(kurl.string().characters(), kurl.string().length());
		}

        EA::WebKit::ViewNotification* const pViewNotification = EA::WebKit::GetViewNotification();
        if(pViewNotification)
            pViewNotification->LoadUpdate(loadInfo);
    }

2011年6月22日 改动

***********************************************************************
************1. Source\EAWebKitFileSystem.cpp
***********************************************************************
Line 39: 
#include "PlatformString.h"
using namespace OWBAL;

Line 156:
由：
pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "wb" : "rb");
修改为：
文件的路径是utf8编码的，并且经过url encode的，所以这里需要把ut8转成ansi
	String fileName = String::fromUTF8(path);

	ULONG cchWideChar = static_cast<ULONG>(fileName.length());
	ULONG cbMultiByte = WideCharToMultiByte(CP_ACP, 0, fileName.charactersWithNullTermination(), cchWideChar, NULL, 0, NULL, NULL);

	char* ansiFileName = new char[cbMultiByte+1];
	EAW_ASSERT(!ansiFileName);
	
	ULONG cchSize = WideCharToMultiByte(CP_ACP, 0, fileName.charactersWithNullTermination(), cchWideChar, ansiFileName, cbMultiByte, NULL, NULL);
	ansiFileName[cbMultiByte] = '\0';

	pFileInfo->mpFile = fopen(ansiFileName, openFlags & kWrite ? "wb" : "rb");

	delete[] ansiFileName;
	ansiFileName = NULL;

***********************************************************************
************2. EAWebKitSupportPackages\EATextEAWebKit\local\source\EATextOutlineFont.cpp
***********************************************************************
Line 1368:
添加了下面代码：
// 主要解决了在设置FontFamilyStandard、FontFamilySerif、FontFamilySansSerif、FontFamilyMonospace、FontFamilyCursive、FamilyFantasy等字体设置成Simsun时个别中文字体显示不正确的，而当设置成带有"True Type Outlines"的标志的字体（如：宋体-方正超大字符集，微软雅黑）等字体时显示正确的问题, 这里主要是根据通过freetype中FT_Render_Glyph函数获取的FT_GlyphSlot::bitmap.pixel_mode类型来设置位图的格式，原来程序是这样设置的：mGlyphBitmap.mBitmapFormat  = (mFontDescription.mSmooth == kSmoothEnabled) ? kBFGrayscale : kBFMonochrome;

switch (static_cast<FT_Pixel_Mode>(pGlyphSlot->bitmap.pixel_mode))
{
case FT_PIXEL_MODE_MONO:
	mGlyphBitmap.mBitmapFormat = kBFMonochrome;
	break;
case FT_PIXEL_MODE_GRAY:
	mGlyphBitmap.mBitmapFormat = kBFGrayscale;
	break;
case FT_PIXEL_MODE_LCD:
case FT_PIXEL_MODE_LCD_V:
	mGlyphBitmap.mBitmapFormat = kBFGrayscale;
	break;
}

***********************************************************************
************************** EAWebkit 1.21.00 ***************************
***********************************************************************
2011年6月17日：

■EAWebkit
	更新到EAWebKit-1.21.00.darkspore版本
	
■主要更新了下面目录：
	1. EAWebKitSupportPackages
	2. include
	3. source
	4. Webkit-owb
	而projects目录下的工程文件(*.vcproj)没有更新
	
■本身自我修改的有以下文件
	1. "include/EAWebKit/EAWebKitConfig.h"
第98行
#ifndef EAWEBKIT_DEBUG
    #if defined(EA_DEBUG) || defined(_DEBUG)
        #define EAWEBKIT_DEBUG 0
    #else
        #define EAWEBKIT_DEBUG 0
    #endif
#endif	

	2. "Webkit-owb/WebCore/page/FrameTree.cpp"
第171行
	// FIXME: this following coding is disabled by achellies to implement to disable the new window created
#if 1
	if (name == "_blank")
		return parent() ? parent() : m_thisFrame;
#else
    // Since "_blank" should never be any frame's name, the following just amounts to an optimization.
    if (name == "_blank")
        return 0;
#endif

	3. "Webkit-owb/BAL/Base/wtf/Platform.h"
第257行
#if !USE(CASQT_UNICODE)
#define WTF_USE_ICU_UNICODE 1
#endif

	4. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingWK.cpp"
第114行
#elif USE(CASQT_UNICODE)
	// added by achellies
    //OwnPtr<TextCodec> textCodec = newTextCodec(*this);
    return newTextCodec(*this)->encode(characters, length, handling);
#endif

	5. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingRegistryWK.cpp" 
第182行
    TextCodecUTF8::registerEncodingNames(addToTextEncodingNameMap);
    TextCodecUTF8::registerCodecs(addToTextCodecMap);

第191行
#if USE(CASQT_UNICODE)
	// added by achellies
    TextCodecCasQt::registerEncodingNames(addToTextEncodingNameMap);
    TextCodecCasQt::registerCodecs(addToTextCodecMap);
#endif
	


***********************************************************************
************************** EAWebkit 1.41.01 ***************************
***********************************************************************
2011年6月16日：

■EAWebkit
	更新到eawebkit_monopolystreets_1_14_01版本

■主要更新了下面目录：
	1. doc
	2. EAWebKitSupportPackages
	3. include
	4. licenses
	5. source
	6. Webkit-owb
	而projects目录下的工程文件(*.vcproj)没有更新

■本身自我修改的有以下文件
	1. "Webkit-owb/WebCore/page/FrameTree.cpp"
第171行
	// FIXME: this following coding is disabled by achellies to implement to disable the new window created
#if 1
	if (name == "_blank")
		return parent() ? parent() : m_thisFrame;
#else
    // Since "_blank" should never be any frame's name, the following just amounts to an optimization.
    if (name == "_blank")
        return 0;
#endif

	2. "include/EAWebKit/EAWebKitConfig.h"
第98行
#ifndef EAWEBKIT_DEBUG
    #if defined(EA_DEBUG) || defined(_DEBUG)
        #define EAWEBKIT_DEBUG 0
    #else
        #define EAWEBKIT_DEBUG 0
    #endif
#endif

	3. "source/EAWebKitFileSystem.cpp"
第152行
#ifdef _MSC_VER
	pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "wb" : "rb");
#else
	pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "w" : "r");
#endif

	4. "Webkit-owb/BAL/Base/wtf/Platform.h"
第257行
#if !USE(CASQT_UNICODE)
#define WTF_USE_ICU_UNICODE 1
#endif

	5. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingWK.cpp"
第114行
#elif USE(CASQT_UNICODE)
	// added by achellies
    //OwnPtr<TextCodec> textCodec = newTextCodec(*this);
    return newTextCodec(*this)->encode(characters, length, handling);
#endif

	6. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingRegistryWK.cpp" 
第182行
    TextCodecUTF8::registerEncodingNames(addToTextEncodingNameMap);
    TextCodecUTF8::registerCodecs(addToTextCodecMap);

第191行
#if USE(CASQT_UNICODE)
	// added by achellies
    TextCodecCasQt::registerEncodingNames(addToTextEncodingNameMap);
    TextCodecCasQt::registerCodecs(addToTextCodecMap);
#endif

