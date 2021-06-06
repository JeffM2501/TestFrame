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
    virtual void ResizeContentArea(const Rectangle& contentArea);

    virtual void Shutdown();

    inline virtual const char* GetViewName() { return nullptr; }
    inline virtual Vector3 GetViewPos() { return Vector3{ 0,0,0 }; }
    inline virtual Vector2 GetViewOrientation() { return Vector2{ 0,0 }; }

public:
    Rectangle LastContentArea = { 0 };

protected:
    virtual void OnShow(const Rectangle& contentArea);
};

RLAPI Shader SetModelMaterialShader(Model* model, int materialIndex, Shader shader);
RLAPI void SetModelMaterialShaderValue(Model* model, int materialIndex, const char* location, const void* value, int uniformType);
RLAPI void SetModelMaterialShaderValueV(Model* model, int materialIndex, const char* location, const void* value, int uniformType, int count);
RLAPI void SetModelMaterialTexture(Model* model, int materialIndex, int maptype, Texture2D texture);

RLAPI Shader LoadShaderSet(const char* resourcePath, const char* name);
RLAPI Shader LoadShaders(const char* resourcePath, const char* vsName, const char* fsName);