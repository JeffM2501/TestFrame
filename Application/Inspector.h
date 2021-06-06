#pragma once

#include "ui_window.h"

#include "raylib.h"

class InspectorWindow : public UIWindow
{
public:
    InspectorWindow() : UIWindow()
    {
        Shown = true;
        Name = "Inspector";
    }

    void OnShow() override
    {
        Vector2 mouse = GetMousePosition();
        ImGui::Text("Mouse X%.0f Y%.0f", mouse.x, mouse.y);
    }
};