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

#include "ui_window.h"

#include "raylib.h"

class SpriteWindow : public UIWindow
{
public:
    SpriteWindow() : UIWindow()
    {
        Shown = false;
        Name = "2D Window";
    }

    void OnShow(MainView* view) override
    {
        Vector2 mouse = GetMousePosition();
        ImGui::Text("Mouse X%.0f Y%.0f", mouse.x, mouse.y);

        Vector3 camPos = view->Camera.GetCameraPosition();
        ImGui::TextUnformatted("Camera");
        ImGui::Text("X % .2f Y % .2f Z % .2f", camPos.x, camPos.y, camPos.z);
        Vector2 camAngles = view->Camera.GetViewAngles();
        ImGui::Text("Yaw%.2f Pitch%.2f", camAngles.y, camAngles.x);
    }
};