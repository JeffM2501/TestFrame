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

#include "main_view.h"

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

MainView::MainView()
{
}

void MainView::Shutdown()
{

}

void MainView::Update()
{

}

void MainView::Show(const Rectangle& contentArea)
{
    OnShow(contentArea);
    LastMousePos = GetMousePosition();
}

void MainView::ResizeContentArea(const Rectangle& contentArea)
{
}

void MainView::OnShow(const Rectangle& contentArea)
{ 
    ClearBackground(BLACK);
}

// shader tools

Shader SetModelMaterialShader(Model* model, int materialIndex, Shader shader)
{
    model->materials[materialIndex].shader = shader;
    return shader;
}

void SetModelMaterialShaderValue(Model* model, int materialIndex, const char* location, const void* value, int uniformType)
{
    Shader shader = model->materials[materialIndex].shader;
    SetShaderValue(shader, GetShaderLocation(shader, location), value, uniformType);
}

void SetModelMaterialShaderValueV(Model* model, int materialIndex, const char* location, const void* value, int uniformType, int count)
{
    Shader shader = model->materials[materialIndex].shader;
    SetShaderValueV(shader, GetShaderLocation(shader, location), value, uniformType, count);
}

void SetModelMaterialTexture(Model* model, int materialIndex, int maptype, Texture2D texture)
{
    SetMaterialTexture(&model->materials[materialIndex], maptype, texture);
}

RLAPI Shader LoadShaderSet(const char* resourcePath, const char* name)
{
    static char vsTemp[512];
    static char fsTemp[512];

#if defined(PLATFORM_WEB)
    static const char* glsl = "glsl110";
#else
    static const char* glsl = "glsl330";
#endif
    sprintf(vsTemp, "%s/%s/%s.vs", resourcePath, glsl, name);
    sprintf(fsTemp, "%s/%s/%s.fs", resourcePath, glsl, name);

    return LoadShader(vsTemp, fsTemp);
}

RLAPI Shader LoadShaders(const char* resourcePath, const char* vsName, const char* fsName)
{
    static char vsTemp[512];
    static char fsTemp[512];

#if defined(PLATFORM_WEB)
    static const char* glsl = "glsl110";
#else
    static const char* glsl = "glsl330";
#endif
    sprintf(vsTemp, "%s/%s/%s.vs", resourcePath, glsl, vsName);
    sprintf(fsTemp, "%s/%s/%s.fs", resourcePath, glsl, fsName);

    return LoadShader(vsTemp, fsTemp);
}