/*
Copyright (C) 2009-2010 Electronic Arts, Inc.  All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1.  Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
2.  Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
3.  Neither the name of Electronic Arts, Inc. ("EA") nor the names of
    its contributors may be used to endorse or promote products derived
    from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY ELECTRONIC ARTS AND ITS CONTRIBUTORS "AS IS" AND ANY
EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ELECTRONIC ARTS OR ITS CONTRIBUTORS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

///////////////////////////////////////////////////////////////////////////////
// EAWebKitSTLWrapper.cpp
// By Arpit Baldeva
///////////////////////////////////////////////////////////////////////////////

#include <EAWebKit/EAWebkitSTLWrapper.h>
#include <EAWebKit/internal/EAWebKitEASTLHelpers.h>
#include <EAWebKit/EAWebKitView.h>
#include <wtf/FastAllocBase.h>

namespace EA
{
	namespace WebKit
	{
		/******************************************EASTLFixedString8Wrapper******************************/
		EASTLFixedString8Wrapper::EASTLFixedString8Wrapper()
			: mString8(0)
		{
			mString8 = EAWEBKIT_NEW("fixedstring8_128") FixedString8_128();
		}

		EASTLFixedString8Wrapper::EASTLFixedString8Wrapper(const char8_t* str)
			: mString8(0)
		{
			mString8 = EAWEBKIT_NEW("fixedstring8_128") FixedString8_128(str);
		}

		EASTLFixedString8Wrapper::EASTLFixedString8Wrapper(const EASTLFixedString8Wrapper& rhs)
		{
			mString8 = EAWEBKIT_NEW("fixedstring8_128") FixedString8_128();
            *(reinterpret_cast<FixedString8_128*>(mString8)) = *(reinterpret_cast<FixedString8_128*> ((rhs).GetImpl()));
		}

		EASTLFixedString8Wrapper& EASTLFixedString8Wrapper::operator =(const EASTLFixedString8Wrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<FixedString8_128*>(mString8);
			mString8 = EAWEBKIT_NEW("fixedstring8_128") FixedString8_128();
			*(reinterpret_cast<FixedString8_128*>(mString8)) = *(reinterpret_cast<FixedString8_128*> ((rhs).GetImpl()));
			
			return *this;
		}

		EASTLFixedString8Wrapper::~EASTLFixedString8Wrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			FixedString8_128* stringPtr = reinterpret_cast<FixedString8_128*>(mString8);
			EAWEBKIT_DELETE stringPtr;
			mString8 = NULL;
		}

		void* EASTLFixedString8Wrapper::GetImpl() const
		{
			return mString8;
		}
/*
		const char8_t* EASTLFixedString8Wrapper::c_str() const
		{
			return reinterpret_cast<FixedString8_128*>(mString8)->c_str();
		}
*/
		/******************************************EASTLFixedString16Wrapper******************************/
		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper()
			: mString16(0)
		{
			mString16 = EAWEBKIT_NEW("fixedstring16_128") FixedString16_128();
		}

		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper(const char16_t* str)
			: mString16(0)
		{
			mString16 = EAWEBKIT_NEW("fixedstring16_128") FixedString16_128(str);
		}

		EASTLFixedString16Wrapper::EASTLFixedString16Wrapper(const EASTLFixedString16Wrapper& rhs)
		{
			mString16 = EAWEBKIT_NEW("fixedstring16_128") FixedString16_128();
            *(reinterpret_cast<FixedString16_128*>(mString16)) = *(reinterpret_cast<FixedString16_128*> ((rhs).GetImpl()));
		}

		EASTLFixedString16Wrapper& EASTLFixedString16Wrapper::operator =(const EASTLFixedString16Wrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<FixedString16_128*>(mString16);
			mString16 = EAWEBKIT_NEW("fixedstring16_128") FixedString16_128();
			*(reinterpret_cast<FixedString16_128*>(mString16)) = *(reinterpret_cast<FixedString16_128*> ((rhs).GetImpl()));

			return *this;
		}
		
		void* EASTLFixedString16Wrapper::GetImpl() const
		{
			return mString16;
		}

/*
		const char16_t* EASTLFixedString16Wrapper::c_str() const
		{
			return reinterpret_cast<FixedString16_128*>(mString16)->c_str();
		}
*/
		EASTLFixedString16Wrapper::~EASTLFixedString16Wrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			FixedString16_128* stringPtr = reinterpret_cast<FixedString16_128*>(mString16);
			EAWEBKIT_DELETE stringPtr;
			mString16 = NULL;
		}

		/******************************************EASTLHeaderMapWrapper******************************/
		EASTLHeaderMapWrapper::EASTLHeaderMapWrapper()
			: mHeaderMap(0)
		{
			mHeaderMap = EAWEBKIT_NEW("HeaderMap") HeaderMap();
		}

		EASTLHeaderMapWrapper::EASTLHeaderMapWrapper(const EASTLHeaderMapWrapper& rhs)
		{
			mHeaderMap = EAWEBKIT_NEW("HeaderMap") HeaderMap();
			*(reinterpret_cast<HeaderMap*>(mHeaderMap)) = *(reinterpret_cast<HeaderMap*> ((rhs).GetImpl()));
		}

		EASTLHeaderMapWrapper& EASTLHeaderMapWrapper::operator =(const EASTLHeaderMapWrapper& rhs)
		{
			if(this == &rhs)
				return *this;

			EAWEBKIT_DELETE reinterpret_cast<HeaderMap*>(mHeaderMap);
			mHeaderMap = EAWEBKIT_NEW("HeaderMap") HeaderMap();
			*(reinterpret_cast<HeaderMap*>(mHeaderMap)) = *(reinterpret_cast<HeaderMap*> ((rhs).GetImpl()));

			return *this;
		}

		EASTLHeaderMapWrapper::~EASTLHeaderMapWrapper()
		{
			//Need to reinterpre_cast here otherwise the destructor won't execute
			HeaderMap* headerMapPtr = reinterpret_cast<HeaderMap*>(mHeaderMap);
			EAWEBKIT_DELETE headerMapPtr;
			mHeaderMap = NULL;
		}
		
		void* EASTLHeaderMapWrapper::GetImpl() const
		{
			return mHeaderMap;
		}
		
	} // namespace WebKit
} // namespace EA