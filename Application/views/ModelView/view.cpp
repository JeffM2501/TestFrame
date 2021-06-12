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
#include "drawing_utils.h"
#include "inspector_window.h"
#include "platform_tools.h"

#include "RaylibColors.h"

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#include <limits>

class ModelViewer : public ThreeDView
{
protected:
    Model ModelFile = { 0 };

    Texture DefaultTexture;

    int SelectedMesh = -1;
    int SelectedMaterial = -1;
    int SelectedMaterialMap = -1;

    bool ShowEmptyMaps = false;

public:
    ModelViewer()
    {
        OpenFileExtensions.push_back({ "obj files","obj" });
        OpenFileExtensions.push_back({ "gltf(binary) files","glb" });
        OpenFileExtensions.push_back({ "gltf files","gltf" });
        OpenFileExtensions.push_back({ "iqm files","iqm" });
    }
    inline const char* GetName() override { return "Model View"; }

    void OnSetup() override
    {
        Image checkerboard = GenImageChecked(512, 512, 64, 64, Colors::RayWhite, Colors::Gray);
        DefaultTexture = LoadTextureFromImage(checkerboard);
        UnloadImage(checkerboard);

        Mesh cube = GenMeshCube(2, 2, 2);
        ModelFile = LoadModelFromMesh(cube);
        ModelFile.materials[0].maps->texture = DefaultTexture;

        ModelFile.transform = MatrixTranslate(0, 1, 0);

        Camera.SetCameraPosition(Vector3{ 0, 1, -5 });
    }

    void OnShutdown() override
    {
        UnloadModel(ModelFile);
    }

    void ShowMaterialMapTreeItem(const MaterialMap& mapItem, const char* name, int materialIndex, int mapIndex)
    {
        bool selected = (SelectedMaterial == materialIndex && SelectedMaterialMap == materialIndex);

        ImGuiSelectableFlags flags = ImGuiSelectableFlags_None;
        if (mapItem.texture.id == 0 && !ShowEmptyMaps)
            return;

        if (ImGui::Selectable(TextFormat("%s###map%d%d", name, mapIndex, materialIndex), &selected, flags))
        {
            SelectedMesh = -1;
            SelectedMaterial = materialIndex;
            SelectedMaterialMap = mapIndex;
        }
    }

    void OnShowInspector(const InspectorWindow& window) override
    {
        ImGui::Checkbox("Show Empty Maps", &ShowEmptyMaps);
        if (ImGui::BeginChild("ModelTree", ImVec2(ImGui::GetContentRegionAvailWidth(), 200), true))
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

                        selected = SelectedMaterial == i;
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
                    }
                    ImGui::TreePop();
                }

                ImGui::TreePop();
            }
            ImGui::EndChild();
        }
        // mesh inspector

        if (SelectedMesh >= 0)
        {
            Inspectors::ShowMeshInspector(ModelFile.meshes[SelectedMesh]);
        }

        // material inspector
        if (SelectedMaterial >= 0)
        {
            Inspectors::ShowMaterialMapInspector(ModelFile.materials[SelectedMaterial].maps[SelectedMaterialMap]);

            if (ImGui::Button("SetTexture"))
            {
                std::string textureFile = PlatformTools::ShowOpenFileDialog(".png");
                if (textureFile.size() > 0)
                {
                    Texture tx = LoadTexture(textureFile.c_str());
                    // TODO, unload the old one?
                    ModelFile.materials[SelectedMaterial].maps[SelectedMaterialMap].texture = tx;
                }
            }
        }
    }

    void OnShow(const Rectangle& contentArea) override
    {
        if (ModelFile.meshCount > 0)
            DrawModel(ModelFile, Vector3Zero(),1, Colors::White);
    }

    void OnFileMenu() override
    {
        std::string file = "*.obj";
        if (OpenFileMenu(file))
        {
            Model newModel = LoadModel(file.c_str());
            if (newModel.meshCount > 0)
            {
                UnloadModel(ModelFile);

                float lowestY = std::numeric_limits<float>::max();

                for (int mesh = 0; mesh < newModel.meshCount; mesh++)
                {
                    float y = GetMeshBoundingBox(newModel.meshes[mesh]).min.y;
                    if (y < lowestY)
                        lowestY = y;
                }

                newModel.transform = MatrixTranslate(0, -lowestY, 0);

                for (int material = 0; material < newModel.materialCount; material++)
                {
                    if (newModel.materials[material].maps[MATERIAL_MAP_ALBEDO].texture.height <= 1)
                        newModel.materials[material].maps[MATERIAL_MAP_ALBEDO].texture = DefaultTexture;
                }

                ModelFile = newModel;
                SelectedMesh = -1;
                SelectedMaterial = -1;
                SelectedMaterialMap = -1;
            }
        }
    }
};

REGISTER_VIEW(ModelViewer);
