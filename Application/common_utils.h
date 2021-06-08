/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   Testframe - a Raylib/ImGui test framework
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2021 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include <ctype.h>
#include "raylib.h"


namespace MovementTools
{
    inline Vector2 GetWADSVector()
    {
        Vector2 result = { 0,0 };
        if (IsKeyDown(KEY_W))
            result.y = -GetFrameTime();
        else if (IsKeyDown(KEY_S))
            result.y = GetFrameTime();

        if (IsKeyDown(KEY_D))
            result.x = GetFrameTime();
        else if (IsKeyDown(KEY_A))
            result.x = -GetFrameTime();

        return result;
    }
}

namespace Base64
{
    size_t GetSize(const char* input);
    void* Decode(const char* input, size_t* size);
    void FreeBuffer(void* buffer);
    char* Encode(const void* input, size_t len);
}

namespace StringUtils
{
    inline char* stristr(const char* str1, const char* str2)
    {
        const char* p1 = str1;
        const char* p2 = str2;
        const char* r = *p2 == 0 ? str1 : 0;

        while (*p1 != 0 && *p2 != 0)
        {
            if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
            {
                if (r == 0)
                {
                    r = p1;
                }

                p2++;
            }
            else
            {
                p2 = str2;
                if (r != 0)
                {
                    p1 = r + 1;
                }

                if (tolower((unsigned char)*p1) == tolower((unsigned char)*p2))
                {
                    r = p1;
                    p2++;
                }
                else
                {
                    r = 0;
                }
            }

            p1++;
        }

        return *p2 == 0 ? (char*)r : 0;
    }
}