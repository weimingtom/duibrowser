	EAWebkit 1.21.00	             
2011年6月17日：

■EAWebkit
> 更新到EAWebKit-1.21.00.darkspore版本

■主要更新了下面目录：
  1. EAWebKitSupportPackages
> 2. include
> 3. source
> 4. Webkit-owb
> 而projects目录下的工程文件(**.vcproj)没有更新**

■本身自我修改的有以下文件
  1. "include/EAWebKit/EAWebKitConfig.h"
第98行
#ifndef EAWEBKIT\_DEBUG
    1. f defined(EA\_DEBUG) || defined(_DEBUG)
      1. efine EAWEBKIT\_DEBUG 0
    1. lse
      1. efine EAWEBKIT\_DEBUG 0
    1. ndif
#endif_

> 2. "Webkit-owb/WebCore/page/FrameTree.cpp"
第171行
> // FIXME: this following coding is disabled by achellies to implement to disable the new window created
#if 1
> if (name == "_blank")
> > return parent() ? parent() : m\_thisFrame;
#else
> > > // Since "_blank" should never be any frame's name, the following just amounts to an optimization.
> > > if (name == "_blank")
> > > > return 0;
#endif_


> 3. "Webkit-owb/BAL/Base/wtf/Platform.h"
第257行
#if !USE(CASQT\_UNICODE)
#define WTF\_USE\_ICU\_UNICODE 1
#endif

> 4. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingWK.cpp"
第114行
#elif USE(CASQT\_UNICODE)
> // added by achellies
> > //OwnPtr

&lt;TextCodec&gt;

 textCodec = newTextCodec(**this);
> > return newTextCodec(**this)->encode(characters, length, handling);
#endif


> 5. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingRegistryWK.cpp"
第182行
> > TextCodecUTF8::registerEncodingNames(addToTextEncodingNameMap);
> > TextCodecUTF8::registerCodecs(addToTextCodecMap);

第191行
#if USE(CASQT\_UNICODE)

> // added by achellies
> > TextCodecCasQt::registerEncodingNames(addToTextEncodingNameMap);
> > TextCodecCasQt::registerCodecs(addToTextCodecMap);
#endif


	EAWebkit 1.14.01	             
2011年6月16日：

■EAWebkit

> 更新到eawebkit\_monopolystreets\_1\_14\_01版本

■主要更新了下面目录：
  1. doc
> 2. EAWebKitSupportPackages
> 3. include
> 4. licenses
> 5. source
> 6. Webkit-owb
> 而projects目录下的工程文件(**.vcproj)没有更新**

■本身自我修改的有以下文件
  1. "Webkit-owb/WebCore/page/FrameTree.cpp"
第171行
> // FIXME: this following coding is disabled by achellies to implement to disable the new window created
#if 1
> if (name == "_blank")
> > return parent() ? parent() : m\_thisFrame;
#else
> > > // Since "_blank" should never be any frame's name, the following just amounts to an optimization.
> > > if (name == "_blank")
> > > > return 0;
#endif_


> 2. "include/EAWebKit/EAWebKitConfig.h"
第98行
#ifndef EAWEBKIT\_DEBUG
    1. f defined(EA\_DEBUG) || defined(_DEBUG)
      1. efine EAWEBKIT\_DEBUG 0
    1. lse
      1. efine EAWEBKIT\_DEBUG 0
    1. ndif
#endif_

> 3. "source/EAWebKitFileSystem.cpp"
第152行
#ifdef _MSC\_VER
> pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "wb" : "rb");
#else
> pFileInfo->mpFile = fopen(path, openFlags & kWrite ? "w" : "r");
#endif_

> 4. "Webkit-owb/BAL/Base/wtf/Platform.h"
第257行
#if !USE(CASQT\_UNICODE)
#define WTF\_USE\_ICU\_UNICODE 1
#endif

> 5. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingWK.cpp"
第114行
#elif USE(CASQT\_UNICODE)
> // added by achellies
> > //OwnPtr

&lt;TextCodec&gt;

 textCodec = newTextCodec(**this);
> > return newTextCodec(**this)->encode(characters, length, handling);
#endif


> 6. "Webkit-owb/BAL/OWBAL/Concretizations/Internationalization/WK/BCTextEncodingRegistryWK.cpp"
第182行
> > TextCodecUTF8::registerEncodingNames(addToTextEncodingNameMap);
> > TextCodecUTF8::registerCodecs(addToTextCodecMap);

第191行
#if USE(CASQT\_UNICODE)

> // added by achellies
> > TextCodecCasQt::registerEncodingNames(addToTextEncodingNameMap);
> > TextCodecCasQt::registerCodecs(addToTextCodecMap);
#endif