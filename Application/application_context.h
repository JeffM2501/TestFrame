#pragma once

#include "main_view.h"

#include "imgui.h"

#include <vector>
#include <string>

struct ApplicationContext
{
    bool Quit = false;
    MainView* View = nullptr;
};

extern ApplicationContext GlobalContext;

namespace LogSink
{
    struct LogItem
    {
        std::string Prefix;
        std::string Text;
        ImVec4 Color = { 1,1,1,1 };
    };

    void Setup();
    bool PopLogLine(LogItem& line);
    void Flush();
}