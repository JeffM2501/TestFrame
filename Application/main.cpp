/*******************************************************************************************
*
*   Application. A testbed
*
*
********************************************************************************************/
#include "application_context.h"
#include "application_ui.h"
#include "main_view.h"

#include "scene_view.h"
#include "sprite_view.h"

#include "rlImGui.h"
#include "raylib.h"
#include "rlImGui.h"

#include <deque>

ApplicationContext GlobalContext;

void ApplicationStartup();
void ApplicationShutdown();

bool ShowStartupLog = true;

#ifdef _WIN32
int wWinMain(void* hInstance, void* hPrevInstance, char* cmdLine, int show)
#else
int main(int argc, char* argv[])
#endif
{
    LogSink::Setup();

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "TestBed Application");
    SetupRLImGui(true);

    ApplicationStartup();

    UIManager ui;
    SceneView sceneView;
    SpriteView spriteView;

    GlobalContext.View = new SceneView();

    ui.Startup();

    // Main game loop
    while (!GlobalContext.Quit && !WindowShouldClose())    // Detect window close button or ESC key
    {
        if (IsWindowResized())
            ui.Resized();

        const Rectangle& contentArea = ui.GetContentArea();

        if (contentArea.x != GlobalContext.View->LastContentArea.x || contentArea.y != GlobalContext.View->LastContentArea.y || contentArea.width != GlobalContext.View->LastContentArea.width || contentArea.height != GlobalContext.View->LastContentArea.height)
        {
            GlobalContext.View->LastContentArea = contentArea;
            GlobalContext.View->ResizeContentArea(GlobalContext.View->LastContentArea);
        }

        ui.Update();
        GlobalContext.View->Update();

        BeginDrawing();
        ClearBackground(DARKGRAY);

        GlobalContext.View->Show(GlobalContext.View->LastContentArea);

        BeginRLImGui();
        ui.Show(GlobalContext.View);
        EndRLImGui();

        EndDrawing();
    }
    GlobalContext.View->Shutdown();
    ui.Shutdown();

    delete(GlobalContext.View);
    GlobalContext.View = nullptr;

    ApplicationShutdown();

    ShutdownRLImGui();
    CloseWindow();
}

void ApplicationStartup()
{
    if (!ShowStartupLog)
        LogSink::Flush();

    TraceLog(LOG_INFO, "Testbed Startup");
}

void ApplicationShutdown()
{
    LogSink::Flush();
}

namespace LogSink
{
    std::deque<LogItem> LogLines;

    void GetLogLevelPrefix(int logLevel, LogItem& item)
    {
        switch (logLevel)
        {
        default:            item.Prefix = "";           item.Color = ImGuiColors::Convert(WHITE); break;
        case LOG_TRACE:     item.Prefix = "Trace: ";    item.Color = ImGuiColors::Convert(GRAY); break;
        case LOG_DEBUG:     item.Prefix = "DEBUG: ";    item.Color = ImGuiColors::Convert(SKYBLUE); break;
        case LOG_INFO:      item.Prefix = "Info: ";     item.Color = ImGuiColors::Convert(GREEN); break;
        case LOG_WARNING:   item.Prefix = "Warning: ";  item.Color = ImGuiColors::Convert(YELLOW); break;
        case LOG_ERROR:     item.Prefix = "ERROR: ";    item.Color = ImGuiColors::Convert(ORANGE); break;
        case LOG_FATAL:     item.Prefix = "FATAL: ";    item.Color = ImGuiColors::Convert(RED); break;
        }
    }

    void TraceLog(int logLevel, const char* text, va_list args)
    {
        static char logText[2048] = { 0 };
        LogItem item;

        GetLogLevelPrefix(logLevel, item);
        vsprintf_s(logText, text, args);
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