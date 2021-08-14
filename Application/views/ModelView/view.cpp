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
#include "model_outliner.h"
#include "platform_tools.h"
#include "shader_manager.h"
#include "texture_manager.h"

#include "RaylibColors.h"

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"

#include <limits>

class ModelViewer : public ThreeDView
{
protected:
    Model ModelFile = { 0 };

    Model sphereModel = { 0 };

    Texture DefaultTexture;

    TextureManager TextureCache;
    ShaderManager ShaderCache;

    std::vector<std::pair<std::string, std::string>> ImageExtensions;
    std::vector<std::pair<std::string, std::string>> VertexShaderExensions;
    std::vector<std::pair<std::string, std::string>> FragmentShaderExtensions;

    std::shared_ptr<ModelOutlinerWindow> Outliner;

public:
    ModelViewer()
    {
        OpenFileExtensions.push_back({ "obj files","obj" });
        OpenFileExtensions.push_back({ "gltf(binary) files","glb" });
        OpenFileExtensions.push_back({ "gltf files","gltf" });
        OpenFileExtensions.push_back({ "iqm files","iqm" });

        ImageExtensions.push_back({ "png files","png" });
        ImageExtensions.push_back({ "bmp files","bmp" });
        ImageExtensions.push_back({ "tga files","tga" });
        ImageExtensions.push_back({ "hdr files","hdr" });

        VertexShaderExensions.push_back({ "vs files","vs" });
        FragmentShaderExtensions.push_back({ "fs files","fs" });
    }
    inline const char* GetName() override { return "Model View"; }

    void OnSetup() override
    {
        Image checkerboard = GenImageChecked(512, 512, 64, 64, Colors::RayWhite, Colors::Gray);
        DefaultTexture = LoadTextureFromImage(checkerboard);
        UnloadImage(checkerboard);

        Mesh cube = GenMeshCube(2, 2, 2);
        ModelFile = LoadModelFromMesh(cube);
        ModelFile.materials[0].maps[MATERIAL_MAP_ALBEDO].texture = DefaultTexture;

        ShaderCache.Clear();
        ShaderCache.LoadShader(0, nullptr, nullptr);

        ModelFile.transform = MatrixTranslate(0, 1, 0);

        Camera.SetCameraPosition(Vector3{ 0, 1, -5 });

        Outliner = std::make_shared<ModelOutlinerWindow>(ModelFile);
        GlobalContext.UI.AddWindow(Outliner);

        sphereModel = LoadModelFromMesh(GenMeshSphere(1, 20, 20));
    }

    void OnShutdown() override
    {
        GlobalContext.UI.RemoveWindow(Outliner);
        Outliner.reset();
        UnloadModel(ModelFile);
    }

    void ShowUniformEditor(ShaderInfo& shader)
    {
        for (auto uniform : shader.Uniforms)
        {
            bool selected = false;
            ImGui::Selectable(uniform.Name.c_str(),selected);
        }
    }

    void OpenShaderFile(bool vertex, int materialIndex)
    {
        std::string filename = PlatformTools::ShowOpenFileDialog(vertex ? "*.vs" : "*.fs", vertex ? VertexShaderExensions : FragmentShaderExtensions);

        if (filename.size() == 0 || !FileExists(filename.c_str()))
            return;

        ShaderInstance& shader = ShaderCache.GetShader(materialIndex);
        if (vertex)
            ModelFile.materials[materialIndex].shader = ShaderCache.LoadShader(materialIndex, filename.c_str(), shader.FragmentShader.GetFileName()).RaylibShader;
        else
            ModelFile.materials[materialIndex].shader = ShaderCache.LoadShader(materialIndex, shader.VertexShader.GetFileName(), filename.c_str()).RaylibShader;
    }

    void OnShowInspector(const InspectorWindow& window) override
    {
        // mesh inspector
        if (Outliner->SelectedMesh >= 0)
        {
            Inspectors::ShowMeshInspector(ModelFile.meshes[Outliner->SelectedMesh]);
        }

        // material inspector
        if (Outliner->SelectedMaterial >= 0)
        {
            // shader
            
            auto shader = ShaderCache.GetShader(Outliner->SelectedMaterial);

            ImGui::TextUnformatted("Shader");
            ImGui::Text("Vertex: %s", shader.VertexShader.Name.c_str());
            ImGui::SameLine();
            if (ImGui::Button("Set###SetVertexShader"))
            {
                OpenShaderFile(true, Outliner->SelectedMaterial);
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear###ClearVertexShader"))
            {
                ShaderInstance& shader = ShaderCache.GetShader(Outliner->SelectedMaterial);
                ModelFile.materials[Outliner->SelectedMaterial].shader = ShaderCache.LoadShader(Outliner->SelectedMaterial, nullptr, shader.FragmentShader.GetFileName()).RaylibShader;
            }
            ShowUniformEditor(shader.VertexShader);

            ImGui::Text("Fragment: %s", shader.FragmentShader.Name.c_str());
            ImGui::SameLine();
            if (ImGui::Button("Set###SetFragmentShader"))
            {
                OpenShaderFile(false, Outliner->SelectedMaterial);
            }
            ImGui::SameLine();
            if (ImGui::Button("Clear###ClearFragmentShader"))
            {
                ShaderInstance& shader = ShaderCache.GetShader(Outliner->SelectedMaterial);
                ModelFile.materials[Outliner->SelectedMaterial].shader = ShaderCache.LoadShader(Outliner->SelectedMaterial, shader.VertexShader.GetFileName(), nullptr).RaylibShader;
            }
            ShowUniformEditor(shader.FragmentShader);


            if (Outliner->SelectedMaterialMap >= 0)
            {
                ImGui::Separator();

                Inspectors::ShowMaterialMapInspector(ModelFile.materials[Outliner->SelectedMaterial].maps[Outliner->SelectedMaterialMap]);

                if (ImGui::Button("SetTexture"))
                {
                    std::string textureFile = PlatformTools::ShowOpenFileDialog(".png", ImageExtensions);
                    if (textureFile.size() > 0)
                    {
                        Texture tx = LoadTexture(textureFile.c_str());
                        int id = ModelFile.materials[Outliner->SelectedMaterial].maps[Outliner->SelectedMaterialMap].texture.id;

                        if (id > 0 && id != DefaultTexture.id)
                            TextureCache.RemoveTexture(ModelFile.materials[Outliner->SelectedMaterial].maps[Outliner->SelectedMaterialMap].texture);

                        ModelFile.materials[Outliner->SelectedMaterial].maps[Outliner->SelectedMaterialMap].texture = tx;
                        TextureCache.AddTexture(tx);
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("Clear###ClearTexture"))
                {
                    int id = ModelFile.materials[Outliner->SelectedMaterial].maps[Outliner->SelectedMaterialMap].texture.id;

                    if (id > 0 && id != DefaultTexture.id)
                        TextureCache.RemoveTexture(ModelFile.materials[Outliner->SelectedMaterial].maps[Outliner->SelectedMaterialMap].texture);

                    ModelFile.materials[Outliner->SelectedMaterial].maps[Outliner->SelectedMaterialMap].texture = DefaultTexture;
                }
            }
        }
    }

    void OnShow(const Rectangle& contentArea) override
    {

        if (ModelFile.meshCount > 0)
            DrawModel(ModelFile, Vector3Zero(), 1, Colors::White);
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
                TextureCache.Clear();
                ShaderCache.Clear();

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
                    newModel.materials[material].shader = ShaderCache.LoadShader(material, nullptr, nullptr).RaylibShader;

                    for (int map = 0; map < MATERIAL_MAP_BRDG; map++)
                    {
                        if (newModel.materials[material].maps[map].texture.height > 1)
                            TextureCache.AddTexture(newModel.materials[material].maps[map].texture);
                        else if (map == 0)
                            newModel.materials[material].maps[map].texture = DefaultTexture;
                    }
                }

                ModelFile = newModel;
                Outliner->SetModel(ModelFile);

            }
        }
    }
};

REGISTER_VIEW(ModelViewer);
