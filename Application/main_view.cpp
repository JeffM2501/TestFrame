#include "main_view.h"

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

MainView::MainView()
{
    Camera.Setup(45, Vector3{ 0,1,0 });
    Camera.MoveSpeed = Vector3{ 5,5,5 };

    rlEnableSmoothLines();
    SetupSkybox();
}

void MainView::Shutdown()
{
    UnloadTexture(SkyboxTexture);
    UnloadModel(Skybox);
}

void MainView::Update()
{

}

void MainView::Show(const Rectangle& contentArea)
{
    if (SceneTexture.texture.id == 0)
        ResizeContentArea(contentArea);

    if (SceneTexture.texture.id == 0)
        return;

    Camera.UseMouseX = Camera.UseMouseY = (IsMouseButtonDown(1) && CheckCollisionPointRec(GetMousePosition(), contentArea));
    Camera.Update();

    BeginTextureMode(SceneTexture);
    OnShow(contentArea);
    EndTextureMode();

    DrawTexturePro(SceneTexture.texture,
        Rectangle{ 0, 0, (float)SceneTexture.texture.width, (float)-SceneTexture.texture.height },
        contentArea,
        Vector2Zero(),
        0,
        WHITE);
}

void MainView::ResizeContentArea(const Rectangle& contentArea)
{
    if (SceneTexture.id != 0)
        UnloadRenderTexture(SceneTexture);

    if (contentArea.width == 0 || contentArea.height == 0)
        return;

    SceneTexture = LoadRenderTexture((int)contentArea.width, (int)contentArea.height);
    BeginTextureMode(SceneTexture);
    ClearBackground(BLACK);
    EndTextureMode();
}

void MainView::Resized()
{

}

void MainView::DrawGizmo(float scale)
{
    DrawLine3D(Vector3{ 0,0,0 }, Vector3{ scale,0,0 }, RED);
    DrawLine3D(Vector3{ scale,0,0 }, Vector3{ scale * 0.75f,0,scale * 0.125f}, RED);

    DrawLine3D(Vector3{ 0,0,0 }, Vector3{ 0,scale,0 }, GREEN);
    DrawLine3D(Vector3{ 0,scale,0 }, Vector3{ scale * 0.125f,scale * 0.75f, 0 }, GREEN);

    DrawLine3D(Vector3{ 0,0,0 }, Vector3{ 0,0,scale }, BLUE);
    DrawLine3D(Vector3{ 0,0,scale }, Vector3{ scale * 0.125f, 0,scale * 0.75f }, BLUE);
}

void MainView::OnShow(const Rectangle& contentArea)
{ 
    ClearBackground(BLACK);
    Camera.BeginMode3D();

    DrawSkybox();

    rlDisableDepthTest();
    DrawGrid(20, 1);
    rlDrawRenderBatchActive();
    rlSetLineWidth(2.0f);
    DrawGizmo(2.0f);
    rlDrawRenderBatchActive();
    rlSetLineWidth(1.0f);
    rlEnableDepthTest();

    Camera.EndMode3D();
}

void MainView::SetupSkybox()
{
    Image skyImg = LoadImage("resources/Daylight Box UV.png");
    SkyboxTexture = LoadTextureCubemap(skyImg, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(skyImg);

    Skybox = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

    int val = MATERIAL_MAP_CUBEMAP;
    // Load skybox shader and set options in required locations
    SetModelMaterialShader(&Skybox, 0, LoadShaderSet("resources/shaders", "skybox"));
    SetModelMaterialShaderValue(&Skybox, 0, "environmentMap", &val, SHADER_UNIFORM_INT);
    val = 1;
    SetModelMaterialShaderValue(&Skybox, 0, "noGamma", &val, SHADER_UNIFORM_INT);
    SetModelMaterialTexture(&Skybox, 0, MATERIAL_MAP_CUBEMAP, SkyboxTexture);
}

void MainView::DrawSkybox()
{
    rlDisableBackfaceCulling();     // Disable backface culling to render inside the cube
    rlDisableDepthMask();			// Disable depth writes
    rlDisableDepthTest();			// Disable depth test for speed

    DrawModel(Skybox, Vector3{ 0, 0, 0 }, 1.0f, WHITE);

    rlEnableBackfaceCulling();     // enable things we turned off
    rlEnableDepthMask();
    rlEnableDepthTest();
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