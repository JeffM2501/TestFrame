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

#include "application_context.h"
#include "scene_view.h"
#include "drawing_utils.h"

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

REGISTER_VIEW(SceneView);

void SceneView::Setup()
{
    Camera.HideCursor = false;
    Camera.Setup(45, Vector3{ 0,1,0 });
    Camera.MoveSpeed = Vector3{ 5,5,5 };

    Camera.ControlsKeys[FPCamera::CameraControls::MOVE_UP] = KEY_SPACE;
    Camera.ControlsKeys[FPCamera::CameraControls::MOVE_DOWN] = KEY_LEFT_CONTROL;

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
        Camera.UseKeyboard = Camera.UseMouseX = Camera.UseMouseY = (IsMouseButtonDown(1) && CheckCollisionPointRec(GetMousePosition(), contentArea));
    else if (!IsMouseButtonDown(1))
        Camera.UseKeyboard = Camera.UseMouseX = Camera.UseMouseY = false;

    Camera.Update();

    BeginTextureMode(SceneTexture);
    if (GlobalContext.ScreenshotView)
    {
        ApplicationContext::Screenshot();
        GlobalContext.ScreenshotView = false;
    }

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

void SceneView::DrawDefaultScene()
{
    DrawSkybox();

    float planeSize = 25.0f;

    rlDisableDepthTest();
    DrawPlane(Vector3Zero(), Vector2{ (float)planeSize, (float)planeSize }, Colors::Brown);
    DrawUtils::DrawGridXZ(Vector3Zero(), planeSize, 1, Colors::Beige, Colors::DarkBrown);
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

void SceneView::OnShow(const Rectangle& contentArea)
{
    ClearBackground(BLACK);
    Camera.BeginMode3D();
    DrawDefaultScene();

    // Draw stuff here

    Camera.EndMode3D();
}