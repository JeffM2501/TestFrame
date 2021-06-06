#include "application_context.h"
#include "ui_window.h"

#include "imgui.h"

void UIWindow::Show(MainView* view)
{
    if (Shown)
    {
        if (ImGui::Begin(Name.c_str(), &Shown, ImGuiWindowFlags_None))
        {
            OnShow(view);
        }
        ImGui::End();
    }
}

void UIWindow::Update()
{
}

void UIWindow::Resize()
{
}


/// <summary>
/// LogWindow 
/// </summary>
LogWindow::LogWindow() : UIWindow()
{
    Shown = true;
    Name = LogWindowName;
}

void LogWindow::OnShow(MainView*)
{
    bool scroollBottom = false;

    LogSink::LogItem item;
    if (LogSink::PopLogLine(item))
    {
        LogLines.emplace_back(std::move(item));

        while (LogLines.size() > 50)
            LogLines.pop_front();

        scroollBottom = true;
    }

    for (auto& line : LogLines)
    {
        ImGui::TextColored(line.Color, "%s", line.Prefix.c_str());
        ImGui::SameLine();
        ImGui::TextUnformatted(line.Text.c_str());
    }

    if (scroollBottom)
        ImGui::SetScrollHereY(1.0f);
}
