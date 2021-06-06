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

    inline const char* GetViewName() const override { return "Scene View"; }
    inline Vector3 GetViewPos() const override { return Camera.GetCameraPosition(); }
    inline Vector2 GetViewOrientation() const override { return Camera.GetViewAngles(); }

    inline bool Is3D() const override { return true; }

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
