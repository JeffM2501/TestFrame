#pragma once

#include "ui_window.h"

#include "raylib.h"

constexpr char InspectorWindowName[] = "Inspector###RaylibInspectorWindow";

class InspectorWindow : public UIWindow
{
public:
    InspectorWindow() : UIWindow()
    {
        Shown = true;
        Name = InspectorWindowName;
    }

    void OnShow(MainView* view) override
    {
        Vector2 mouse = GetMousePosition();
        ImGui::Text("Mouse X%.0f Y%.0f", mouse.x, mouse.y);

        Vector3 camPos = view->GetViewPos();
        ImGui::Text("%s", view->GetViewName());
        ImGui::Text("X % .2f Y % .2f Z % .2f", camPos.x, camPos.y, camPos.z);
        Vector2 camAngles = view->GetViewOrientation();
        ImGui::Text("Yaw%.2f Pitch%.2f", camAngles.y, camAngles.x);
    }
};