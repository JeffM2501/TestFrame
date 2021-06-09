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
#include "raymath.h"


namespace MovementTools
{
    inline Vector2 GetWADSVector(float speed = 1, bool flipY = false)
    {
        float yScale = 1;
        if (flipY)
            yScale = -1;
        Vector2 result = { 0,0 };
        if (IsKeyDown(KEY_W))
            result.y = -GetFrameTime() * speed * yScale;
        else if (IsKeyDown(KEY_S))
            result.y = GetFrameTime() * speed * yScale;

        if (IsKeyDown(KEY_D))
            result.x = GetFrameTime() * speed;
        else if (IsKeyDown(KEY_A))
            result.x = -GetFrameTime() * speed;

        return result;
    }

    inline Vector2 GetArrowVector(float speed = 1, bool flipY = false)
    {
        float yScale = 1;
        if (flipY)
            yScale = -1;
        Vector2 result = { 0,0 };
        if (IsKeyDown(KEY_UP))
            result.y = -GetFrameTime() * speed * yScale;
        else if (IsKeyDown(KEY_DOWN))
            result.y = GetFrameTime() * speed * yScale;

        if (IsKeyDown(KEY_RIGHT))
            result.x = GetFrameTime() * speed;
        else if (IsKeyDown(KEY_LEFT))
            result.x = -GetFrameTime() * speed;

        return result;
    }

    inline Vector2 RotateVector2Arrows(Vector2& vector, float speed = 90)
    {
        if (IsKeyDown(KEY_LEFT))
            vector = Vector2Rotate(vector, -speed * GetFrameTime());
        if (IsKeyDown(KEY_RIGHT))
            vector = Vector2Rotate(vector, speed * GetFrameTime());

        return vector;
    }
}

namespace KeyboardTools
{
    inline bool IsShiftDown()
    {
        return IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_RIGHT_SHIFT);
    }

    inline bool IsControlDown()
    {
        return IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
    }

    inline bool IsAltDown()
    {
        return IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);
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