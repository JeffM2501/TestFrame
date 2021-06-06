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

#include "common_utils.h"

#include "raylib.h"

namespace Base64
{
    static const unsigned char base64Table[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 62, 0, 0, 0, 63, 52, 53,
        54, 55, 56, 57, 58, 59, 60, 61, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 2, 3, 4,
        5, 6, 7, 8, 9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24,
        25, 0, 0, 0, 0, 0, 0, 26, 27, 28,
        29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48,
        49, 50, 51
    };

    int GetSize(const char* input)
    {
        int size = 0;

        for (int i = 0; input[4 * i] != 0; i++)
        {
            if (input[4 * i + 3] == '=')
            {
                if (input[4 * i + 2] == '=') size += 1;
                else size += 2;
            }
            else size += 3;
        }

        return size;
    } 
    
    void FreeBuffer(void* buffer)
    {
        MemFree(buffer);
    }

    void* Decode(const char* input, int* size)
    {
        *size = GetSize(input);

        unsigned char* buf = (unsigned char*)MemAlloc(*size);
        for (int i = 0; i < *size / 3; i++)
        {
            unsigned char a = base64Table[(int)input[4 * i]];
            unsigned char b = base64Table[(int)input[4 * i + 1]];
            unsigned char c = base64Table[(int)input[4 * i + 2]];
            unsigned char d = base64Table[(int)input[4 * i + 3]];

            buf[3 * i] = (a << 2) | (b >> 4);
            buf[3 * i + 1] = (b << 4) | (c >> 2);
            buf[3 * i + 2] = (c << 6) | d;
        }

        if (*size % 3 == 1)
        {
            int n = *size / 3;
            unsigned char a = base64Table[(int)input[4 * n]];
            unsigned char b = base64Table[(int)input[4 * n + 1]];
            buf[*size - 1] = (a << 2) | (b >> 4);
        }
        else if (*size % 3 == 2)
        {
            int n = *size / 3;
            unsigned char a = base64Table[(int)input[4 * n]];
            unsigned char b = base64Table[(int)input[4 * n + 1]];
            unsigned char c = base64Table[(int)input[4 * n + 2]];
            buf[*size - 2] = (a << 2) | (b >> 4);
            buf[*size - 1] = (b << 4) | (c >> 2);
        }
        return buf;
    }
}