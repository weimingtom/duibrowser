***********************************************************************
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

