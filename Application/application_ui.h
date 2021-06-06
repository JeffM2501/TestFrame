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