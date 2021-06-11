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

#include "main_view.h"

class SpriteView : public MainView 
{
public:
    Texture Tx;

public:
    inline const char* GetName() override { return "2D View"; }
    void Setup() override;

    virtual void Shutdown();

    virtual void Update();
    virtual void Show(const Rectangle& contentArea);
    void ResizeContentArea(const Rectangle& contentArea) override;

    const char* GetViewName() const override { return "2D View"; }
    inline Vector3 GetViewPos() const override { return Vector3{ Camera.target.x, Camera.target.y, 0 }; }
    inline Vector2 GetViewOrientation() const override { return Vector2{ Camera.rotation, 0 }; }

    inline Vector2 GetWorldMousePos()
    {
        Vector2 pos = GetViewMousePosition();
        return GetScreenToWorld2D(pos, Camera);
    }

    void ShowInspectorContents(const InspectorWindow& window) override;

protected:
    virtual void OnShow(const Rectangle& contentArea);

protected:
    RenderTexture SceneTexture = { 0 };
    Camera2D Camera = { 0 };

    bool Dragging = false;
    Vector2 ClickPos = { 0,0 };
    Vector2 ClickTarget = { 0,0 };

    static const int MaxZoomLevels = 14;
    float ZoomLevels[MaxZoomLevels] = { 0.125f, 0.25f, 0.5f, 1, 1.5f, 2.0f, 3.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 14.0f, 16.0f };
    int ZoomLevel = 3;
};