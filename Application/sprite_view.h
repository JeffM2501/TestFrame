#pragma once

#include "main_view.h"

class SpriteView : public MainView 
{
public:
    SpriteView();

    virtual void Shutdown();

    virtual void Update();
    virtual void Show(const Rectangle& contentArea);
    void ResizeContentArea(const Rectangle& contentArea) override;

    const char* GetViewName() const override { return "2D View"; }

protected:
    virtual void OnShow(const Rectangle& contentArea);

protected:
    RenderTexture SceneTexture = { 0 };

    Texture Tx;
};