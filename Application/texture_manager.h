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
#include "raylib.h"

#include <map>

class TextureInstance
{
public:
    size_t RefCount = 0;
    Texture2D Tx;
};

class TextureManager
{
public:
    std::map<int, TextureInstance> TextureCache;

    void AddTexture(Texture tx)
    {
        if (TextureCache.find(tx.id) != TextureCache.end())
            TextureCache[tx.id] = TextureInstance{ 0,tx };

        TextureCache[tx.id].RefCount++;
    }

    void RemoveTexture(Texture tx)
    {
        std::map<int, TextureInstance>::iterator itr = TextureCache.find(tx.id);
        if (itr == TextureCache.end())
            return;

        itr->second.RefCount--;
        if (itr->second.RefCount <= 0)
        {
            UnloadTexture(itr->second.Tx);
            TextureCache.erase(itr);
        }
    }

    void Clear()
    {
        for (auto& tx : TextureCache)
            UnloadTexture(tx.second.Tx);

        TextureCache.clear();
    }
};

