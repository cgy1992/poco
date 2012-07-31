//
// UnicodeConverter.cpp
//
// $Id: //poco/1.4/Foundation/src/UnicodeConverter.cpp#1 $
//
// Library: Foundation
// Package: Text
// Module:  UnicodeConverter
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef POCO_NO_WSTRING


#include "Poco/UnicodeConverter.h"
#include "Poco/TextConverter.h"
#include "Poco/TextIterator.h"
#include "Poco/UTF8Encoding.h"
#include "Poco/UTF16Encoding.h"
#include "Poco/UTF32Encoding.h"
#include <cstring>
#include <wchar.h>


namespace Poco {


void UnicodeConverter::toWideUTF(const std::string& utf8String, std::wstring& utfWideString)
{
	if (sizeof(wchar_t) == 2)
	{
		toUTF16(utf8String, utfWideString);
	}
	else
	{
		toUTF32(utf8String, utfWideString);
	}
}


void UnicodeConverter::toWideUTF(const char* utf8String, int length, std::wstring& utfWideString)
{
	if (sizeof(wchar_t) == 2)
	{
		toUTF16(utf8String, utfWideString);
	}
	else
	{
		toUTF32(utf8String, utfWideString);
	}
}


void UnicodeConverter::toWideUTF(const char* utf8String, std::wstring& utfWideString)
{
	if (sizeof(wchar_t) == 2)
	{
		toUTF16(utf8String, utfWideString);
	}
	else
	{
		toUTF32(utf8String, utfWideString);
	}
}


void UnicodeConverter::toUTF32(const std::string& utf8String, std::wstring& utf32String)
{
	utf32String.clear();
	UTF8Encoding utf8Encoding;
	TextIterator it(utf8String, utf8Encoding);
	TextIterator end(utf8String);

	while (it != end)
	{
		int cc = *it++;
		utf32String += (wchar_t) cc;
	}
}


void UnicodeConverter::toUTF32(const char* utf8String, int length, std::wstring& utf32String)
{
	poco_check_ptr (utf8String);

	utf32String.clear();

	UTF8Encoding utf8Encoding;
	UTF32Encoding utf32Encoding;
	const unsigned char* it  = (const unsigned char*) utf8String;
	const unsigned char* end = (const unsigned char*) utf8String + length;

	while (it < end)
	{
		int n = utf8Encoding.queryConvert(it, 1);
		int uc;
		int read = 1;

		while (-1 > n && (end - it) >= -n)
		{
			read = -n;
			n = utf8Encoding.queryConvert(it, read);
		}

		if (-1 > n)
		{
			it = end;
		}
		else
		{
			it += read;
		}

		if (-1 >= n)
		{
			uc = 0xfffd;	// Replacement Character (instead of '?')
		}
		else
		{
			uc = n;
		}

		utf32String += (wchar_t) uc;
	}
}


void UnicodeConverter::toUTF32(const char* utf8String, std::wstring& utf32String)
{
	toUTF32(utf8String, (int) std::strlen(utf8String), utf32String);
}


void UnicodeConverter::toUTF16(const std::string& utf8String, std::wstring& utf16String)
{
	utf16String.clear();
	UTF8Encoding utf8Encoding;
	TextIterator it(utf8String, utf8Encoding);
	TextIterator end(utf8String);
	while (it != end) 
	{
		int cc = *it++;
		if (cc <= 0xffff)
		{
			utf16String += (wchar_t) cc;
		}
		else
		{
			cc -= 0x10000;
			utf16String += (wchar_t) ((cc >> 10) & 0x3ff) | 0xd800;
			utf16String += (wchar_t) (cc & 0x3ff) | 0xdc00;
		}
	}
}


void UnicodeConverter::toUTF16(const char* utf8String, int length, std::wstring& utf16String)
{
	poco_check_ptr (utf8String);

	utf16String.clear();

	UTF8Encoding utf8Encoding;
	UTF16Encoding utf16Encoding;
	const unsigned char* it  = (const unsigned char*) utf8String;
	const unsigned char* end = (const unsigned char*) utf8String + length;
	
	while (it < end)
	{
		int n = utf8Encoding.queryConvert(it, 1);
		int uc;
		int read = 1;

		while (-1 > n && (end - it) >= -n)
		{
			read = -n;
			n = utf8Encoding.queryConvert(it, read);
		}
		
		if (-1 > n)
		{
			it = end;
		}
		else
		{
			it += read;
		}

		if (-1 >= n)
		{
			uc = 0xfffd;	// Replacement Character (instead of '?')
		}
		else
		{
			uc = n;
		}

		if (uc > 0xffff)
		{
			uc -= 0x10000;
			utf16String += (wchar_t) ((uc >> 10) & 0x3ff) | 0xd800 ;
			utf16String += (wchar_t) (uc & 0x3ff) | 0xdc00 ;
		}
		else
		{
			utf16String += (wchar_t) uc;
		}
	}
}


void UnicodeConverter::toUTF16(const char* utf8String, std::wstring& utf16String)
{
	toUTF16(utf8String, (int) std::strlen(utf8String), utf16String);
}


void UnicodeConverter::toUTF8(const std::wstring& utf16String, std::string& utf8String)
{
	utf8String.clear();
	UTF8Encoding utf8Encoding;
	if (sizeof(wchar_t) == 2)
	{
		UTF16Encoding utf16Encoding;
		TextConverter converter(utf16Encoding, utf8Encoding);
		converter.convert(utf16String.data(), (int) utf16String.length() * sizeof(wchar_t), utf8String);
	}
	else
	{
		UTF32Encoding utf32Encoding;
		TextConverter converter(utf32Encoding, utf8Encoding);
		converter.convert(utf16String.data(), (int) utf16String.length() * sizeof(wchar_t), utf8String);
	}
}


void UnicodeConverter::toUTF8(const wchar_t* utf16String, int length, std::string& utf8String)
{
	utf8String.clear();
	UTF8Encoding utf8Encoding;
	if (sizeof(wchar_t) == 2)
	{
		UTF16Encoding utf16Encoding;
		TextConverter converter(utf16Encoding, utf8Encoding);
		converter.convert(utf16String, (int) length * sizeof(wchar_t), utf8String);
	}
	else
	{
		UTF32Encoding utf32Encoding;
		TextConverter converter(utf32Encoding, utf8Encoding);
		converter.convert(utf16String, (int) length * sizeof(wchar_t), utf8String);
	}
}


void UnicodeConverter::toUTF8(const wchar_t* utf16String, std::string& utf8String)
{
	toUTF8(utf16String, (int) wcslen(utf16String), utf8String);
}


} // namespace Poco


#endif // POCO_NO_WSTRING
