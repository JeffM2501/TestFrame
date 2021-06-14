/**********************************************************************************************
*
*   raylibExtras * Utilities and Shared Components for Raylib
*
*   Testframe - a Raylib/ImGui test framework
*
*   LICENSE: ZLIB
*
*   Copyright (c) 2021 Jeffery Myers
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy
*   of this software and associated documentation files (the "Software"), to deal
*   in the Software without restriction, including without limitation the rights
*   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
*   copies of the Software, and to permit persons to whom the Software is
*   furnished to do so, subject to the following conditions:
*
*   The above copyright notice and this permission notice shall be included in all
*   copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
*   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
*   SOFTWARE.
*
**********************************************************************************************/

#pragma once

#include "raylib.h"
#include "FPCamera.h"
#include "imgui.h"

class InspectorWindow;

class MainView
{
public:
    MainView();
    virtual ~MainView() = default;

    inline virtual const char* GetName() { return nullptr; }
    virtual void Setup() {}

    virtual void Update();
    virtual void Show(const Rectangle& contentArea);
    virtual void ResizeContentArea(const Rectangle& contentArea);

    virtual void Shutdown();

    inline virtual const char* GetViewName() const { return nullptr; }
    inline virtual Vector3 GetViewPos() const { return Vector3{ 0,0,0 }; }
    inline virtual Vector2 GetViewOrientation() const { return Vector2{ 0,0 }; }

    virtual void ShowInspectorContents(const InspectorWindow& window) {}

    // menu functions
    inline virtual void OnFileMenu() {}
    inline virtual void OnMenuBar() {}
    inline virtual void OnToolsMenu() {}

    inline Vector2 GetViewMousePosition()
    {
        Vector2 pos = GetMousePosition();

        pos.x -= LastContentArea.x;
        pos.y -= LastContentArea.y;
        return pos;
    }

    inline Vector2 GetMouseDelta()
    {
        Vector2 pos = GetMousePosition();

        pos.x -= LastMousePos.x;
        pos.y -= LastMousePos.y;
        return pos;
    }

public:
    Rectangle LastContentArea = { 0 };

    std::vector<std::pair<std::string, std::string>> OpenFileExtensions;

protected:
    virtual void OnShow(const Rectangle& contentArea);

    bool OpenFileMenu(std::string& filename);

private:
    Vector2 LastMousePos = { 0,0 };

};

class ThreeDView : public MainView
{
protected:
    RenderTexture SceneTexture = { 0 };

    FPCamera Camera;
    TextureCubemap SkyboxTexture = { 0 };
    Model Skybox = { 0 };

    // options
    std::string SkyboxResource = "Daylight Box UV.png";

    bool ShowSkybox = true;
    bool ShowGround = true;
    bool ShowOrigin = true;

    virtual void OnSetup() {}
    virtual void OnShutdown() {}
    virtual void OnShowInspector(const InspectorWindow& window) {}
    inline virtual void OnUpdate() {};
public:
    void Setup() override;
    void Shutdown() override;
    void Show(const Rectangle& contentArea) override;
    void ResizeContentArea(const Rectangle& contentArea) override;
    void ShowInspectorContents(const InspectorWindow& window) override;

    inline void Update() override { OnUpdate(); }
protected:
    void DrawGizmo(float scale = 1);
    void SetupSkybox();
    void DrawSkybox();
    void DrawDefaultScene();
};

// 2d View

class TwoDView : public MainView
{
protected:
    virtual void OnSetup() {}
    virtual void OnShutdown() {}
    virtual void OnShowInspector(const InspectorWindow& window) {}
    inline virtual void OnUpdate() {};

public:
    void Setup() override;

    virtual void Shutdown();

    void Update() override;
    virtual void Show(const Rectangle& contentArea);
    void ResizeContentArea(const Rectangle& contentArea) override;

    inline Vector3 GetViewPos() const override { return Vector3{ Camera.target.x, Camera.target.y, 0 }; }
    inline Vector2 GetViewOrientation() const override { return Vector2{ Camera.rotation, 0 }; }

    inline Vector2 GetWorldMousePos()
    {
        Vector2 pos = GetViewMousePosition();
        return GetScreenToWorld2D(pos, Camera);
    }

    void ShowInspectorContents(const InspectorWindow& window) override;

protected:
    virtual void OnShow(const Rectangle& contentArea) {}

protected:
    RenderTexture SceneTexture = { 0 };
    Camera2D Camera = { 0 };

    bool Dragging = false;
    Vector2 ClickPos = { 0,0 };
    Vector2 ClickTarget = { 0,0 };

    static const int MaxZoomLevels = 14;
    float ZoomLevels[MaxZoomLevels] = { 0.125f, 0.25f, 0.5f, 1, 1.5f, 2.0f, 3.0f, 4.0f, 6.0f, 8.0f, 10.0f, 12.0f, 14.0f, 16.0f };
    int ZoomLevel = 3;
};


RLAPI Shader SetModelMaterialShader(Model* model, int materialIndex, Shader shader);
RLAPI void SetModelMaterialShaderValue(Model* model, int materialIndex, const char* location, const void* value, int uniformType);
RLAPI void SetModelMaterialShaderValueV(Model* model, int materialIndex, const char* location, const void* value, int uniformType, int count);
RLAPI void SetModelMaterialTexture(Model* model, int materialIndex, int maptype, Texture2D texture);

RLAPI Shader LoadShaderSet(const char* resourcePath, const char* name);
RLAPI Shader LoadShaders(const char* resourcePath, const char* vsName, const char* fsName);