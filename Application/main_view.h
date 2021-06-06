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

    virtual void Shutdown();

    FPCamera Camera;
protected:
    virtual void OnShow(const Rectangle& contentArea);
    void DrawGizmo(float scale = 1);

    void SetupSkybox();
    void DrawSkybox();

protected:
    RenderTexture SceneTexture = { 0 };


    TextureCubemap SkyboxTexture = { 0 };
    Model Skybox = { 0 };
};

RLAPI Shader SetModelMaterialShader(Model* model, int materialIndex, Shader shader);
RLAPI void SetModelMaterialShaderValue(Model* model, int materialIndex, const char* location, const void* value, int uniformType);
RLAPI void SetModelMaterialShaderValueV(Model* model, int materialIndex, const char* location, const void* value, int uniformType, int count);
RLAPI void SetModelMaterialTexture(Model* model, int materialIndex, int maptype, Texture2D texture);

RLAPI Shader LoadShaderSet(const char* resourcePath, const char* name);
RLAPI Shader LoadShaders(const char* resourcePath, const char* vsName, const char* fsName);