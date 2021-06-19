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

#include "ui_window.h"

#include "raylib.h"
#include "rlImGui.h"
#include "raylib.h"


namespace Inspectors
{
    inline void ShowTextureInspector(const Texture& texture, float width = 0)
    {
        if (width <= 0)
        width = ImGui::GetContentRegionAvailWidth();

        float height = width * (texture.height / (float)texture.width);
        RLImGuiImageSize(&texture, (int)width, (int)height);
        ImGui::Text("ID:%d W:%d H:%d", texture.id, texture.width, texture.height);
        if (texture.mipmaps > 1)
            ImGui::Text("Mipmap Levels:%d", texture.mipmaps);
    }

    inline void ShowSetTextureFilter(const Texture& texture)
    {
        if (ImGui::Button("Point"))
            SetTextureFilter(texture, TEXTURE_FILTER_POINT);
        
        ImGui::SameLine();
        if (ImGui::Button("Bi"))
            SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);

        ImGui::SameLine(); 
        if (ImGui::Button("Tri"))
            SetTextureFilter(texture, TEXTURE_FILTER_TRILINEAR);

        ImGui::SameLine(); 
        if (ImGui::Button("A4x"))
            SetTextureFilter(texture, TEXTURE_FILTER_ANISOTROPIC_4X);

        ImGui::SameLine(); 
        if (ImGui::Button("A8x"))
            SetTextureFilter(texture, TEXTURE_FILTER_ANISOTROPIC_8X);

        ImGui::SameLine(); 
        if (ImGui::Button("A16x"))
            SetTextureFilter(texture, TEXTURE_FILTER_ANISOTROPIC_16X);

        ImGui::SameLine();
        if (ImGui::Button("Mip"))
            GenTextureMipmaps((Texture*)&texture);

    }

    inline void ShowMeshInspector(const Mesh& mesh)
    {
        if (ImGui::BeginChild("Mesh Inspector"))
        {
            ImGui::TextUnformatted("Mesh");
            ImGui::Separator();

            ImGui::Text("Verts: %d Tris:%d", mesh.vertexCount, mesh.triangleCount);
            ImGui::EndChild();
        }
    }

    inline void ShowMaterialMapInspector(MaterialMap& materialMap)
    {
        ImGui::TextUnformatted("Material Map");
        ImGui::Separator();
        ImColor color = ImGuiColors::Convert(materialMap.color);
        if (ImGui::ColorEdit4("Color###MatColor", &(color.Value.x), ImGuiColorEditFlags_NoBorder))
        {
            materialMap.color.r = (unsigned char)(255 * color.Value.x);
            materialMap.color.g = (unsigned char)(255 * color.Value.y);
            materialMap.color.b = (unsigned char)(255 * color.Value.z);
            materialMap.color.a = (unsigned char)(255 * color.Value.w);
        }
        ImGui::Separator();
        ImGui::DragFloat("Value", &materialMap.value, 0.001f, 0, 1.0f);
        ImGui::Separator();
        ShowTextureInspector(materialMap.texture, 150);
    }
}

constexpr char InspectorWindowName[] = " Inspector###RaylibInspectorWindow";

class InspectorWindow : public UIWindow
{
public:
    InspectorWindow() : UIWindow()
    {
        Shown = true;
    }

    inline void GetName(std::string& name, MainView* view) const override 
    { 
        name = view->GetName();
        name += InspectorWindowName;
    }

    inline const char* GetMenuName() const override { return "Inspector"; }

    inline void ShowCommonData(MainView* view) const
    {
        float frameTime = GetFrameTime();
        float instantFPS = 0;
        if (frameTime > 0)
            instantFPS = 1 / frameTime;

        int avgFPS = GetFPS();

        ImGui::Text("FPS %d:Avg %.0f:Inst DT:%.2fms", avgFPS, instantFPS, frameTime * 1000);
        Vector2 mouse = view->GetViewMousePosition();
        ImGui::Text("Mouse X%.0f Y%.0f", mouse.x, mouse.y);
    }

    inline void OnShow(MainView* view) override
    {
        view->ShowInspectorContents(*this);
    }
};
