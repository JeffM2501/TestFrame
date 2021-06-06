#include "scene_view.h"

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

SceneView::SceneView()
{
    Camera.HideCursor = false;
    Camera.Setup(45, Vector3{ 0,1,0 });
    Camera.MoveSpeed = Vector3{ 5,5,5 };

    rlEnableSmoothLines();
    SetupSkybox();
}

void SceneView::Shutdown()
{
    UnloadTexture(SkyboxTexture);
    UnloadModel(Skybox);
}

void SceneView::Update()
{

}

void SceneView::Show(const Rectangle& contentArea)
{
    if (SceneTexture.texture.id == 0)
        ResizeContentArea(contentArea);

    if (SceneTexture.texture.id == 0)
        return;

    if (IsMouseButtonPressed(1))
        Camera.UseMouseX = Camera.UseMouseY = (IsMouseButtonDown(1) && CheckCollisionPointRec(GetMousePosition(), contentArea));
    else if (!IsMouseButtonDown(1))
        Camera.UseMouseX = Camera.UseMouseY = false;

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

void SceneView::ResizeContentArea(const Rectangle& contentArea)
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

void SceneView::DrawGizmo(float scale)
{
    DrawLine3D(Vector3{ 0,0,0 }, Vector3{ scale,0,0 }, RED);
    DrawLine3D(Vector3{ scale,0,0 }, Vector3{ scale * 0.75f,0,scale * 0.125f }, RED);

    DrawLine3D(Vector3{ 0,0,0 }, Vector3{ 0,scale,0 }, GREEN);
    DrawLine3D(Vector3{ 0,scale,0 }, Vector3{ scale * 0.125f,scale * 0.75f, 0 }, GREEN);

    DrawLine3D(Vector3{ 0,0,0 }, Vector3{ 0,0,scale }, BLUE);
    DrawLine3D(Vector3{ 0,0,scale }, Vector3{ scale * 0.125f, 0,scale * 0.75f }, BLUE);
}

void SceneView::OnShow(const Rectangle& contentArea)
{
    ClearBackground(BLACK);
    Camera.BeginMode3D();
    DrawDefaultScene();
   
    // Draw stuff here

    Camera.EndMode3D();
}

void SceneView::DrawDefaultScene()
{
    DrawSkybox();

    int planeSize = 20;

    rlDisableDepthTest();
    DrawPlane(Vector3Zero(), Vector2{ (float)planeSize, (float)planeSize }, DARKBROWN);
    DrawGrid(planeSize, 1);
    rlDrawRenderBatchActive();
    rlSetLineWidth(2.0f);
    DrawGizmo(2.0f);
    rlDrawRenderBatchActive();
    rlSetLineWidth(1.0f);
    rlEnableDepthTest();
}

void SceneView::SetupSkybox()
{
    Image skyImg = LoadImage("Daylight Box UV.png");
    SkyboxTexture = LoadTextureCubemap(skyImg, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(skyImg);

    Skybox = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));

    int val = MATERIAL_MAP_CUBEMAP;
    // Load skybox shader and set options in required locations
    SetModelMaterialShader(&Skybox, 0, LoadShaderSet("shaders", "skybox"));
    SetModelMaterialShaderValue(&Skybox, 0, "environmentMap", &val, SHADER_UNIFORM_INT);
    val = 1;
    SetModelMaterialShaderValue(&Skybox, 0, "noGamma", &val, SHADER_UNIFORM_INT);
    SetModelMaterialTexture(&Skybox, 0, MATERIAL_MAP_CUBEMAP, SkyboxTexture);
}

void SceneView::DrawSkybox()
{
    rlDisableBackfaceCulling();     // Disable backface culling to render inside the cube
    rlDisableDepthMask();			// Disable depth writes
    rlDisableDepthTest();			// Disable depth test for speed

    DrawModel(Skybox, Vector3{ 0, 0, 0 }, 1.0f, WHITE);

    rlEnableBackfaceCulling();     // enable things we turned off
    rlEnableDepthMask();
    rlEnableDepthTest();
}
