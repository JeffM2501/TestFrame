#pragma once

#include "raylib.h"
#include "FPCamera.h"

class MainView
{
public:
    MainView();
    virtual ~MainView() {}

    virtual void Update();
    virtual void Show(const Rectangle& contentArea);
    virtual void Resized();
    virtual void ResizeContentArea(const Rectangle& contentArea);

protected:
    virtual void OnShow(const Rectangle& contentArea);
    void DrawGizmo(float scale = 1);
protected:
    RenderTexture SceneTexture = { 0 };
    FPCamera Camera;
};