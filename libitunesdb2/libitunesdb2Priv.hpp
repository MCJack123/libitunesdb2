//
//  libitunesdb2Priv.hpp
//  libitunesdb2
//
//  Created by JackMacWindows on 12/28/18.
//  Copyright © 2018 MCJack123. All rights reserved.
//

/* The classes below are not exported */
#include <stdint.h>
#include <locale>
#include <codecvt>
#define STRING_ENCODING_UTF8 0
#define STRING_ENCODING_UTF16 1
#define STRING_ENCODING_UTF16LE 2
#define STRING_ENCODING_UTF32 3
#define STRING_ENCODING_UTF32LE 4

#pragma GCC visibility push(hidden)

extern int String_GetEncoding(char *string);
extern std::u16string utf8to16(std::string str);
extern std::string utf16to8(std::u16string str);

#pragma GCC visibility pop
