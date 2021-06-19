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


constexpr char ModelOutlinerWindowName[] = " Model Outline###RaylibModelOutlinerWindow";

class ModelOutlinerWindow : public UIWindow
{
protected:
    Model& ModelFile;
    bool ShowEmptyMaps = false;

public:
    int SelectedMesh = -1;
    int SelectedMaterial = -1;
    int SelectedMaterialMap = -1;

    ModelOutlinerWindow(Model& model) 
        : UIWindow()
        , ModelFile(model)
    {
        Shown = true;
    }

    inline void GetName(std::string& name, MainView* view) const override
    {
        name = ModelOutlinerWindowName;
    }

    inline void SetModel(Model& model)
    { 
        ModelFile = model;
        SelectedMesh = -1;
        SelectedMaterial = -1;
        SelectedMaterialMap = -1;
    }

    inline const char* GetMenuName() const override { return "Outline"; }

    inline void ShowMaterialMapTreeItem(const MaterialMap& mapItem, const char* name, int materialIndex, int mapIndex)
    {
        bool selected = (SelectedMaterial == materialIndex && SelectedMaterialMap == mapIndex);

        char* suffix = "";

        ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;
        if (mapItem.texture.id == 0)
        {
            if (!ShowEmptyMaps)
                return;
            else
                suffix = "(*)";
        }

        if (ImGui::Selectable(TextFormat("%s%s###mat%d-map%d", name,suffix, materialIndex, mapIndex ), &selected, flags))
        {
            SelectedMesh = -1;
            SelectedMaterial = materialIndex;
            SelectedMaterialMap = mapIndex;
        }
    }

    inline void OnShow(MainView* view) override
    {
        if (ImGui::BeginChild("ModelTree", ImVec2(ImGui::GetContentRegionAvailWidth(), ImGui::GetContentRegionAvail().y - 30), true))
        {
            bool selected = false;
            if (ImGui::TreeNodeEx("Model Tree", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::TreeNodeEx("Meshes", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    for (int i = 0; i < ModelFile.meshCount; i++)
                    {
                        selected = SelectedMesh == i;
                        if (ImGui::Selectable(TextFormat("%d###mesh%d", i, i), &selected, ImGuiSelectableFlags_None))
                        {
                            SelectedMesh = i;
                            SelectedMaterial = -1;
                            SelectedMaterialMap = -1;
                        }
                    }
                    ImGui::TreePop();
                }

                if (ImGui::TreeNodeEx("Materials", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    for (int i = 0; i < ModelFile.materialCount; i++)
                    {
                        Material& mat = ModelFile.materials[i];

                        if (ImGui::TreeNodeEx(TextFormat("%d###material%d", i, i), ImGuiTreeNodeFlags_DefaultOpen))
                        {
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_ALBEDO], "Albedo", i, MATERIAL_MAP_ALBEDO);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_METALNESS], "Metalness", i, MATERIAL_MAP_METALNESS);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_NORMAL], "Normal", i, MATERIAL_MAP_NORMAL);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_ROUGHNESS], "Roughness", i, MATERIAL_MAP_ROUGHNESS);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_OCCLUSION], "Occlusion", i, MATERIAL_MAP_OCCLUSION);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_EMISSION], "Emission", i, MATERIAL_MAP_EMISSION);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_HEIGHT], "Height", i, MATERIAL_MAP_HEIGHT);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_CUBEMAP], "CubeMap", i, MATERIAL_MAP_CUBEMAP);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_IRRADIANCE], "Irradiance", i, MATERIAL_MAP_IRRADIANCE);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_PREFILTER], "Prefilter", i, MATERIAL_MAP_PREFILTER);
                            ShowMaterialMapTreeItem(mat.maps[MATERIAL_MAP_BRDG], "BRDG", i, MATERIAL_MAP_BRDG);
                            ImGui::TreePop();
                        }
                        if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0))
                            SelectedMaterial = i;
                    }
                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
            ImGui::EndChild();
        }
        ImGui::Checkbox("Show Empty Maps", &ShowEmptyMaps);
    }
};
