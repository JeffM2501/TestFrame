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
    virtual ~MainView() {}

    inline virtual const char* GetName() { return nullptr; }
    virtual void Setup() {}

    virtual void Update();
    virtual void Show(const Rectangle& contentArea);
    virtual void ResizeContentArea(const Rectangle& contentArea);

    virtual void Shutdown();

    inline virtual const char* GetViewName() const { return nullptr; }
    inline virtual Vector3 GetViewPos() const { return Vector3{ 0,0,0 }; }
    inline virtual Vector2 GetViewOrientation() const { return Vector2{ 0,0 }; }

    virtual void ShowInspectorContents(const InspectorWindow& window);

    inline virtual bool Is3D() const { return false; }

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

protected:
    virtual void OnShow(const Rectangle& contentArea);

private:
    Vector2 LastMousePos = { 0,0 };

};

RLAPI Shader SetModelMaterialShader(Model* model, int materialIndex, Shader shader);
RLAPI void SetModelMaterialShaderValue(Model* model, int materialIndex, const char* location, const void* value, int uniformType);
RLAPI void SetModelMaterialShaderValueV(Model* model, int materialIndex, const char* location, const void* value, int uniformType, int count);
RLAPI void SetModelMaterialTexture(Model* model, int materialIndex, int maptype, Texture2D texture);

RLAPI Shader LoadShaderSet(const char* resourcePath, const char* name);
RLAPI Shader LoadShaders(const char* resourcePath, const char* vsName, const char* fsName);