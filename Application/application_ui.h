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
#include "imgui.h"

#include <vector>
#include <memory>

class UIManager
{
public:
    void Startup();
    void Shutdown();
    void Update();
    void Show(MainView* view = nullptr);
    void Resized();

    inline const Rectangle& GetContentArea() { return ContentArea; }

protected:
    void SetupUI();
    void ShowMenu();

protected:
    ImGuiID DockspaceId;

    bool LogWindowOpen = true;
  
    std::vector<std::shared_ptr<UIWindow>> Windows;

private:
    bool ShowStyleEditor = false;
    bool ShowDemoWindow = false;
    bool ShowMetricsWindow = false;
    bool ShowAboutImGuiWindow = false;
    bool ShowAboutWindow = false;

    Rectangle ContentArea = { 0 };

private:
    void ShowDebugWindows();
};