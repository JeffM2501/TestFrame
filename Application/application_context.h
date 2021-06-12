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

#include "imgui.h"

#include <vector>
#include <string>

struct ApplicationContext
{
    bool Quit = false;
    MainView* View = nullptr;

    std::vector<MainView*> RegisteredViews;

    bool ScreenshotView = false;
    bool TakeScreenshot = false;
    bool CopyScreenshot = false;

    static void Screenshot();

    void ChangeView(MainView* newView)
    {
        if (View != nullptr)
            View->Shutdown();

        View = newView;
        if (View != nullptr)
            View->Setup();
    }

    MainView* FindView(const char* name)
    {
        if (name == nullptr)
            return RegisteredViews[0];

        std::string _name = name;
        for (MainView* view : RegisteredViews)
        {
            std::string _vName = view->GetName();
            if (_name == _vName)
                return view;
        }

        return RegisteredViews[0];
    }
};

extern ApplicationContext GlobalContext;

#define REGISTER_VIEW(viewType) \
static viewType View;


namespace LogSink
{
    struct LogItem
    {
        int Level = 0;
        std::string Prefix;
        std::string Text;
        ImVec4 Color = { 1,1,1,1 };
    };

    void Setup();
    bool PopLogLine(LogItem& line);
    void Flush();

    inline const char* GetLogLevelName(int logLevel)
    {
        switch (logLevel)
        {
        default:            return "All";
        case LOG_TRACE:     return "Trace";
        case LOG_DEBUG:     return "DEBUG";
        case LOG_INFO:      return "Info";
        case LOG_WARNING:   return "Warning";
        case LOG_ERROR:     return "ERROR";
        case LOG_FATAL:     return "FATAL";
        }
    }
}