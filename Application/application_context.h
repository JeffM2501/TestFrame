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

#include "application_ui.h"

#include "RLAssets.h"
#include "imgui.h"

#include <vector>
#include <string>

class MainView;

class ApplicationPrefs
{
protected:
    std::string PrefsFile;

public:
    std::string LastView = "2d View";
    int WindowHeight = 720;
    int WindowWidth = 1280;

    bool Maximized = false;

    void Setup()
    {
        PrefsFile = rlas_GetApplicationBasePath();
        PrefsFile += "prefs.txt";

        FILE* fp = fopen(PrefsFile.c_str(), "r");
        if (fp == nullptr)
            return;

        char temp[1024] = { 0 };
        int max = 0;
        char* p = temp;
        while (true)
        {
            fread(p, 1, 1, fp);
            if (*p == '\n')
            {
                *p = '\0';
                break;
            }
            else
                p++; 
        }
        fscanf(fp, "%d\n%d\n%d", &WindowHeight,&WindowWidth,&max);
        LastView = temp;
        Maximized = max != 0;
        fclose(fp);
    }

    void Save()
    {
        FILE* fp = fopen(PrefsFile.c_str(), "w");
        if (fp == nullptr)
            return;

        Maximized = IsWindowMaximized();
        if (!Maximized)
        {
            WindowHeight = GetScreenHeight();
            WindowWidth = GetScreenWidth();
        }

        int max = Maximized ? 1 : 0;

        fprintf(fp, "%s\n%d\n%d\n%d", LastView.c_str(), WindowHeight, WindowWidth, max);
        fclose(fp);
    }
};

struct ApplicationContext
{
    bool Quit = false;
    MainView* View = nullptr;
    UIManager UI;
    std::vector<MainView*> RegisteredViews;

    ApplicationPrefs Prefs;

    bool ScreenshotView = false;
    bool TakeScreenshot = false;
    bool CopyScreenshot = false;

    static void Screenshot();

    void ChangeView(MainView* newView);

    MainView* FindView(const char* name);
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