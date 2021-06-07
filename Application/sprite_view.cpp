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

#include "sprite_view.h"
#include "drawing_utils.h"


SpriteView::SpriteView() : MainView()
{
    Tx = LoadTexture("parrots.png");
    SetTextureFilter(Tx, TEXTURE_FILTER_TRILINEAR);
    Camera.zoom = 1;
}

void SpriteView::Shutdown()
{
    UnloadTexture(Tx);
}

void SpriteView::Update()
{
    if (IsMouseButtonPressed(1))
    {
        Dragging = true;
        ClickPos = GetMousePosition();
        ClickTarget = Camera.target;
    }

    if (IsMouseButtonDown(1))
    {
        Vector2 delta = Vector2Scale(Vector2Subtract(ClickPos, GetMousePosition()), 1.0f/ZoomLevels[ZoomLevel]);
        Camera.target = Vector2Add(ClickTarget, delta);
    }
}

void SpriteView::Show(const Rectangle& contentArea)
{
    if (SceneTexture.texture.id == 0)
        ResizeContentArea(contentArea);

    if (SceneTexture.texture.id == 0)
        return;

    if (CheckCollisionPointRec(GetMousePosition(), contentArea))
    {
        if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT))
        {
            Camera.zoom += GetMouseWheelMove() * 0.05f;
            if (Camera.zoom <= 0)
                Camera.zoom = 0.05f;
        }
        else
        {
            if (GetMouseWheelMove() < 0)
            {
                ZoomLevel--;
                if (ZoomLevel < 0)
                    ZoomLevel = 0;
            }
            else if (GetMouseWheelMove() > 0)
            {
                ZoomLevel++;
                if (ZoomLevel >= MaxZoomLevels)
                    ZoomLevel = MaxZoomLevels - 1;
            }

            if (GetMouseWheelMove() != 0)
                Camera.zoom = ZoomLevels[ZoomLevel];

        }

    }

    BeginTextureMode(SceneTexture);
    OnShow(contentArea);
    EndTextureMode();

    DrawTexturePro(SceneTexture.texture,
        Rectangle{ 0, 0, (float)SceneTexture.texture.width, (float)-SceneTexture.texture.height },
        contentArea,
        Vector2Zero(),
        0,
        Colors::White);
}

void SpriteView::ResizeContentArea(const Rectangle& contentArea)
{
    if (SceneTexture.id != 0)
        UnloadRenderTexture(SceneTexture);

    if (contentArea.width == 0 || contentArea.height == 0)
        return;

    Camera.offset = RectTools::CenterSize(contentArea);

    SceneTexture = LoadRenderTexture((int)contentArea.width, (int)contentArea.height);
    BeginTextureMode(SceneTexture);
    ClearBackground(Colors::Black);
    EndTextureMode();
}

void SpriteView::OnShow(const Rectangle& contentArea)
{
    ClearBackground(Colors::DarkGray);
    BeginMode2D(Camera);
    DrawUtils::DrawGrid2D(Vector2Zero(), (int)RectTools::MaxSize(contentArea)/2, 100, Colors::Gray, Colors::DarkBlue);
    DrawTexture(Tx, Tx.width/-2, Tx.height/-2, Colors::White);
    EndMode2D();

    DrawText(TextFormat("Zoom:%.1f%%", Camera.zoom * 100), 0, (int)contentArea.height - 20, 20, Colors::White);
}
