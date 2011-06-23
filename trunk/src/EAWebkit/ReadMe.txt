2011��6��23�ոĶ�
***********************************************************************
************1. E:\Webkit\Codes\DuiBrowser\src\EAWebkit\Webkit-owb\WebKit\OrigynWebBrowser\WebCoreSupport\WebFrameLoaderClient.cpp
***********************************************************************
Line 421��
����ViewNotification���ղ�����ҳ�����bug
�������´��룺
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

2011��6��22�� �Ķ�

***********************************************************************
************1. Source\EAWebKitFileSystem.cpp
***********************************************************************
Line 39: 
#include "PlatformString.h"
using namespace OWBAL;

Line 156:
�ɣ�
pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "wb" : "rb");
�޸�Ϊ��
�ļ���·����utf8����ģ����Ҿ���url encode�ģ�����������Ҫ��ut8ת��ansi
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
�����������룺
// ��Ҫ�����������FontFamilyStandard��FontFamilySerif��FontFamilySansSerif��FontFamilyMonospace��FontFamilyCursive��FamilyFantasy���������ó�Simsunʱ��������������ʾ����ȷ�ģ��������óɴ���"True Type Outlines"�ı�־�����壨�磺����-���������ַ�����΢���źڣ�������ʱ��ʾ��ȷ������, ������Ҫ�Ǹ���ͨ��freetype��FT_Render_Glyph������ȡ��FT_GlyphSlot::bitmap.pixel_mode����������λͼ�ĸ�ʽ��ԭ���������������õģ�mGlyphBitmap.mBitmapFormat  = (mFontDescription.mSmooth == kSmoothEnabled) ? kBFGrayscale : kBFMonochrome;

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
2011��6��17�գ�

��EAWebkit
	���µ�EAWebKit-1.21.00.darkspore�汾
	
����Ҫ����������Ŀ¼��
	1. EAWebKitSupportPackages
	2. include
	3. source
	4. Webkit-owb
	��projectsĿ¼�µĹ����ļ�(*.vcproj)û�и���
	
�����������޸ĵ��������ļ�
	1. "include/EAWebKit/EAWebKitConfig.h"
��98��
#ifndef EAWEBKIT_DEBUG
    #if defined(EA_DEBUG) || defined(_DEBUG)
        #define EAWEBKIT_DEBUG 0
    #else
        #define EAWEBKIT_DEBUG 0
    #endif
#endif	

	2. "Webkit-owb/WebCore/page/FrameTree.cpp"
��171��
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
��257��
#if !USE(CASQT_UNICODE)
#define WTF_USE_ICU_UNICODE 1
#endif

	4. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingWK.cpp"
��114��
#elif USE(CASQT_UNICODE)
	// added by achellies
    //OwnPtr<TextCodec> textCodec = newTextCodec(*this);
    return newTextCodec(*this)->encode(characters, length, handling);
#endif

	5. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingRegistryWK.cpp" 
��182��
    TextCodecUTF8::registerEncodingNames(addToTextEncodingNameMap);
    TextCodecUTF8::registerCodecs(addToTextCodecMap);

��191��
#if USE(CASQT_UNICODE)
	// added by achellies
    TextCodecCasQt::registerEncodingNames(addToTextEncodingNameMap);
    TextCodecCasQt::registerCodecs(addToTextCodecMap);
#endif
	


***********************************************************************
************************** EAWebkit 1.41.01 ***************************
***********************************************************************
2011��6��16�գ�

��EAWebkit
	���µ�eawebkit_monopolystreets_1_14_01�汾

����Ҫ����������Ŀ¼��
	1. doc
	2. EAWebKitSupportPackages
	3. include
	4. licenses
	5. source
	6. Webkit-owb
	��projectsĿ¼�µĹ����ļ�(*.vcproj)û�и���

�����������޸ĵ��������ļ�
	1. "Webkit-owb/WebCore/page/FrameTree.cpp"
��171��
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
��98��
#ifndef EAWEBKIT_DEBUG
    #if defined(EA_DEBUG) || defined(_DEBUG)
        #define EAWEBKIT_DEBUG 0
    #else
        #define EAWEBKIT_DEBUG 0
    #endif
#endif

	3. "source/EAWebKitFileSystem.cpp"
��152��
#ifdef _MSC_VER
	pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "wb" : "rb");
#else
	pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "w" : "r");
#endif

	4. "Webkit-owb/BAL/Base/wtf/Platform.h"
��257��
#if !USE(CASQT_UNICODE)
#define WTF_USE_ICU_UNICODE 1
#endif

	5. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingWK.cpp"
��114��
#elif USE(CASQT_UNICODE)
	// added by achellies
    //OwnPtr<TextCodec> textCodec = newTextCodec(*this);
    return newTextCodec(*this)->encode(characters, length, handling);
#endif

	6. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingRegistryWK.cpp" 
��182��
    TextCodecUTF8::registerEncodingNames(addToTextEncodingNameMap);
    TextCodecUTF8::registerCodecs(addToTextCodecMap);

��191��
#if USE(CASQT_UNICODE)
	// added by achellies
    TextCodecCasQt::registerEncodingNames(addToTextEncodingNameMap);
    TextCodecCasQt::registerCodecs(addToTextCodecMap);
#endif

