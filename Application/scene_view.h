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
