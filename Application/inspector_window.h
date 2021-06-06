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

    void OnShow(MainView* view) override
    {
        Vector2 mouse = GetMousePosition();
        ImGui::Text("Mouse X%.0f Y%.0f", mouse.x, mouse.y);

        Vector3 camPos = view->Camera.GetCameraPosition();
        ImGui::TextUnformatted("Camera");
        ImGui::Text("X % .2f Y % .2f Z % .2f", camPos.x, camPos.y, camPos.z);
        Vector2 camAngles = view->Camera.GetViewAngles();
        ImGui::Text("Yaw%.2f Pitch%.2f", camAngles.y, camAngles.x);
    }
};