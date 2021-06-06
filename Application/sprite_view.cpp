#include "sprite_view.h"


SpriteView::SpriteView() : MainView()
{
    Tx = LoadTexture("resources/parrots.png");
}

void SpriteView::Shutdown()
{
    UnloadTexture(Tx);
}

void SpriteView::Update()
{

}

void SpriteView::Show(const Rectangle& contentArea)
{
    if (SceneTexture.texture.id == 0)
        ResizeContentArea(contentArea);

    if (SceneTexture.texture.id == 0)
        return;

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

void SpriteView::ResizeContentArea(const Rectangle& contentArea)
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

void SpriteView::OnShow(const Rectangle& contentArea)
{
    ClearBackground(DARKGRAY);

    DrawTexture(Tx, 10, 10, WHITE);
}
