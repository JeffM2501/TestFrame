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
#include "application_ui.h"
#include "main_view.h"

#include "platform_tools.h"

#include "raylib.h"
#include "rlgl.h"
#include "rlImGui.h"
#include "RLAssets.h"

#include <deque>

ApplicationContext GlobalContext;

void ApplicationContext::Screenshot()
{
    if (GlobalContext.CopyScreenshot)
    {
        GlobalContext.CopyScreenshot = false;
        unsigned char* imgData = rlReadScreenPixels(GetScreenWidth(), GetScreenHeight());
        Image image = { imgData, GetScreenWidth(), GetScreenHeight(), 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };

        PlatformTools::CopyImageToClipboard(image);

        RL_FREE(imgData);
        TraceLog(LOG_INFO, "Copied Screenshot to Clipboard");
    }

    if (GlobalContext.TakeScreenshot)
    {
        GlobalContext.TakeScreenshot = false;
        std::string path = PlatformTools::ShowSaveFileDialog(TextFormat("%d.png", GetRandomValue(1, 999999999)));
        if (path.size() > 0)
            ::TakeScreenshot(path.c_str());
    }
}

void ApplicationContext::ChangeView(MainView* newView)
{
    if (View != nullptr)
        View->Shutdown();

    View = newView;
    if (View != nullptr)
        View->Setup();

    if (View != nullptr)
    {
        Prefs.LastView = View->GetName();
        Prefs.Save();
    }
}

MainView* ApplicationContext::FindView(const char* name)
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

void ApplicationStartup();
void ApplicationShutdown();

bool ShowStartupLog = true;
bool Start2D = false;

#ifdef _WIN32
int wWinMain(void* hInstance, void* hPrevInstance, char* cmdLine, int show)
#else
int main(int argc, char* argv[])
#endif
{
    LogSink::Setup();

    GlobalContext.Prefs.Setup();

    unsigned int flags = FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE;

    if (GlobalContext.Prefs.Maximized)
        flags |= FLAG_WINDOW_MAXIMIZED;

    SetConfigFlags(flags);
    InitWindow(GlobalContext.Prefs.WindowWidth, GlobalContext.Prefs.WindowHeight, "TestBed Application");
   
    if (GlobalContext.Prefs.Maximized)
        MaximizeWindow();

    ApplicationStartup();


    GlobalContext.ChangeView(GlobalContext.FindView(GlobalContext.Prefs.LastView.c_str()));
    GlobalContext.UI.Startup();

    // Main game loop
    while (!GlobalContext.Quit && !WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsWindowResized())
            GlobalContext.UI.Resized();

        const Rectangle& contentArea = GlobalContext.UI.GetContentArea();

        if (contentArea.x != GlobalContext.View->LastContentArea.x || contentArea.y != GlobalContext.View->LastContentArea.y || contentArea.width != GlobalContext.View->LastContentArea.width || contentArea.height != GlobalContext.View->LastContentArea.height)
        {
            GlobalContext.View->LastContentArea = contentArea;
            GlobalContext.View->ResizeContentArea(GlobalContext.View->LastContentArea);
        }

        GlobalContext.UI.Update();
        GlobalContext.View->Update();

        BeginDrawing();
        ClearBackground(DARKGRAY);

        GlobalContext.View->Show(GlobalContext.View->LastContentArea);

        BeginRLImGui();
        GlobalContext.UI.Show(GlobalContext.View);
        EndRLImGui();

        EndDrawing();

        if (!GlobalContext.ScreenshotView)
            ApplicationContext::Screenshot();
    }

    GlobalContext.Prefs.Save();

    GlobalContext.ChangeView(nullptr);
    GlobalContext.UI.Shutdown();

    ApplicationShutdown();

    ShutdownRLImGui();
    CloseWindow();
}

void ApplicationStartup()
{
    if (!ShowStartupLog)
        LogSink::Flush();

    TraceLog(LOG_INFO, "Testframe Startup");

    rlas_SetAssetRootPath("resources/",false);

    InitRLGLImGui();

    // load fonts here
    FinishRLGLImguSetup();
}

void ApplicationShutdown()
{
    rlas_Cleanup();
    LogSink::Flush();
}

namespace LogSink
{
    std::deque<LogItem> LogLines;

    void GetLogLevelPrefix(int logLevel, LogItem& item)
    {
        item.Prefix = GetLogLevelName(logLevel);
        item.Prefix += ": ";
        switch (logLevel)
        {
        default:            item.Prefix.clear(); item.Color = ImGuiColors::Convert(WHITE); break;
        case LOG_TRACE:     item.Color = ImGuiColors::Convert(GRAY); break;
        case LOG_DEBUG:     item.Color = ImGuiColors::Convert(SKYBLUE); break;
        case LOG_INFO:      item.Color = ImGuiColors::Convert(GREEN); break;
        case LOG_WARNING:   item.Color = ImGuiColors::Convert(YELLOW); break;
        case LOG_ERROR:     item.Color = ImGuiColors::Convert(ORANGE); break;
        case LOG_FATAL:     item.Color = ImGuiColors::Convert(RED); break;
        }
    }

    void TraceLog(int logLevel, const char* text, va_list args)
    {
        static char logText[2048] = { 0 };
        LogItem item;
        item.Level = logLevel;
        GetLogLevelPrefix(logLevel, item);
        vsprintf(logText, text, args);
        item.Text += logText;
        LogLines.push_back(item);
    }

    void Setup()
    {
        SetTraceLogCallback(TraceLog);
    }

    bool PopLogLine(LogItem& line)
    {
        if (LogLines.size() == 0)
            return false;

        line = LogLines.front();
        LogLines.pop_front();
        return true;
    }

    void Flush()
    {
        LogLines.clear();
    }
}