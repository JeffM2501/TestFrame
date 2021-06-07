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
#include "common_utils.h"
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
    ImGui::TextUnformatted("Show:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    if (ImGui::BeginCombo("##LogLevel", LogSink::GetLogLevelName(ShowLevel)))
    {
        for (int i = 0; i < LOG_NONE; ++i)
        {
            bool is_selected = i == ShowLevel;
            if (ImGui::Selectable(LogSink::GetLogLevelName(i), is_selected))
                ShowLevel = i;

            if (is_selected)
                ImGui::SetItemDefaultFocus();
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputTextWithHint("###filterText", "Filter", FilterText, 512);

    ImGui::SameLine();
    bool copy = false;
    if(ImGui::Button("Copy"))
    {
        copy = true;
    }

    ImGui::SameLine();
    if (ImGui::Button("Clear"))
    {
        LogSink::Flush();
        LogLines.clear();
    }

    if (ImGui::BeginChild("###LogChild", ImGui::GetContentRegionAvail()))
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

        std::string copyBuffer;

        for (auto& line : LogLines)
        {
            if (ShowLevel != 0 && ShowLevel != line.Level)
                continue;

            if (FilterText[0] != '\0')
            {
                if (StringUtils::stristr(line.Text.c_str(), FilterText) == nullptr)
                    continue;
            }

            ImGui::TextColored(line.Color, "%s", line.Prefix.c_str());
            ImGui::SameLine();
            ImGui::TextUnformatted(line.Text.c_str());

            if (copy)
                copyBuffer += line.Prefix + line.Text + "\r\n";
        }

        if (copy)
        {
            SetClipboardText(copyBuffer.c_str());
        }

        if (scroollBottom)
            ImGui::SetScrollHereY(1.0f);

        ImGui::EndChild();
    }
}
