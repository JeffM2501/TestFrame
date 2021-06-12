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

#include "application_context.h"
#include "inspector_window.h"
#include "main_view.h"
#include "drawing_utils.h"

class SpriteView : public TwoDView
{
protected:
    Texture Tx;
public:
    inline const char* GetName() override { return "2D View"; }

    void OnSetup() override
    {
        Tx = LoadTexture("parrots.png");
    }

    void OnShutdown() override
    {
        UnloadTexture(Tx);
    }

    void OnShow(const Rectangle& contentArea) override
    {
        DrawTexture(Tx, Tx.width / -2, Tx.height / -2, Colors::White);
    }

    void OnShowInspector(const InspectorWindow& window) override
    {
        ImGui::TextUnformatted("Texture");
        Inspectors::ShowTextureInspector(Tx);
        Inspectors::ShowSetTextureFilter(Tx);
    }
};

REGISTER_VIEW(SpriteView);