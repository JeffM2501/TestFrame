#pragma once

#include "main_view.h"

class SceneView : public MainView
{
public:
    SceneView();
    virtual ~SceneView() {}

    void Update() override;
    void Show(const Rectangle& contentArea) override;
    void ResizeContentArea(const Rectangle& contentArea) override;

    void Shutdown() override;

    const char* GetViewName() override { return "Scene View"; }
    Vector3 GetViewPos() override { return Camera.GetCameraPosition(); }
    Vector2 GetViewOrientation() override { return Camera.GetViewAngles(); }

protected:
    void OnShow(const Rectangle& contentArea) override;
    void DrawGizmo(float scale = 1);

    void SetupSkybox();
    void DrawSkybox();
    void DrawDefaultScene();

protected:
    RenderTexture SceneTexture = { 0 };

    FPCamera Camera;
    TextureCubemap SkyboxTexture = { 0 };
    Model Skybox = { 0 };
};
