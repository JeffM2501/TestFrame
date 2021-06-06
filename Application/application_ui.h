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
    void Update();
    void Show();
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

    Rectangle ContentArea = { 0 };

private:
    void ShowDebugWindows();
};