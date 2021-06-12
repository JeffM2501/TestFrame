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

#include <memory>
#include <string>
#include <deque>

#include "application_context.h"
#include "main_view.h"

class UIWindow
{
public:
    UIWindow() {}
    virtual ~UIWindow(){}

    bool Shown = false;
    inline virtual void GetName(std::string& name, MainView* view) const { name = "BaseWindow"; }
    inline virtual const char* GetMenuName() const { return "Base"; }

    void Show(MainView* view = nullptr);

    virtual void Update();
    virtual void Resize();

    virtual void Shutdown() {}

protected:
    virtual void OnShow(MainView* view) {}
    std::string Name;
};

constexpr char LogWindowName[] = "Log###RaylibLogWindow";

class LogWindow : public UIWindow
{
public:
    LogWindow();
    inline void GetName(std::string& name, MainView* view) const override { name = LogWindowName; }
    inline const char* GetMenuName() const override { return "Log"; }
    void OnShow(MainView* view) override;

private:
    std::deque<LogSink::LogItem> LogLines;

    int ShowLevel = 0;

    char FilterText[512] = { 0 };
};