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
}

void MainView::ResizeContentArea(const Rectangle& contentArea)
{
}

void MainView::Resized()
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