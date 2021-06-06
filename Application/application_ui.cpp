#include "application_context.h"
#include "application_ui.h"

#include "Inspector.h"

#include "raylib.h"
#include "imgui_internal.h"


void UIManager::Startup()
{
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    ImGui::GetIO().ConfigWindowsMoveFromTitleBarOnly = true;

    ImGui::GetStyle().WindowMenuButtonPosition = ImGuiDir_Right;

    Windows.emplace_back(std::make_shared<LogWindow>());
    Windows.emplace_back(std::make_shared<InspectorWindow>());
}

void UIManager::Resized()
{
    for (auto& window : Windows)
        window->Resize();
}

void UIManager::Show()
{
    ImVec2 screenSize((float)GetScreenWidth(), (float)GetScreenHeight());

    ImGui::SetNextWindowSize(screenSize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2());

    if (ImGui::Begin("MainFrame",
        nullptr,
        ImGuiWindowFlags_MenuBar |
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoDecoration |
        ImGuiWindowFlags_NoSavedSettings))
    {
     ImGui::PopStyleVar();

        DockspaceId = ImGui::DockSpace(ImGui::GetID("MainWindowDock"), ImGui::GetContentRegionAvail() ,ImGuiDockNodeFlags_PassthruCentralNode | ImGuiDockNodeFlags_NoDockingInCentralNode);

        if (ImGui::IsWindowAppearing())
            SetupUI();

        for (auto& window : Windows)
            window->Show(); 

        auto* rootNode = ImGui::DockBuilderGetNode(DockspaceId);
        if (rootNode != nullptr && rootNode->CentralNode != nullptr)
        {
            ContentArea.x = rootNode->CentralNode->Pos.x;
            ContentArea.y = rootNode->CentralNode->Pos.y;
            ContentArea.width = rootNode->CentralNode->Size.x;
            ContentArea.height = rootNode->CentralNode->Size.y;
        }
        ShowDebugWindows();
        ShowMenu();
        ImGui::End();
    }
    else
    {
        ImGui::PopStyleVar();
    }
}

void UIManager::Update()
{
    for (auto& window : Windows)
        window->Update();
}

void UIManager::SetupUI()
{
    auto* rootNode = ImGui::DockBuilderGetNode(DockspaceId);
    if (rootNode == nullptr || rootNode->ChildNodes[0] == nullptr)
    {
        auto childId = ImGui::DockBuilderSplitNode(DockspaceId, ImGuiDir_Down, 0.25f, nullptr, nullptr);

        ImGui::DockBuilderDockWindow(LogWindowName, childId);
    }
}

void UIManager::ShowMenu()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit", "Alt+F4"))
                GlobalContext.Quit = true;

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            for (auto& window : Windows)
            {
                ImGui::MenuItem(window->Name.c_str(), nullptr, &window->Shown);
            }
            
            ImGui::EndMenu();
        } 
        
        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::BeginMenu("ImGui"))
            {
                if (ImGui::MenuItem("Item Picker"))
                    ImGui::DebugStartItemPicker();

                ImGui::MenuItem("Style Editor", nullptr, &ShowStyleEditor);
                ImGui::MenuItem("Metrics", nullptr, &ShowMetricsWindow);
                ImGui::MenuItem("Demo", nullptr, &ShowDemoWindow);

                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void UIManager::ShowDebugWindows()
{
    if (ShowStyleEditor)
    {
        auto& style = ImGui::GetStyle();
        if (ImGui::Begin("Style Editor", &ShowStyleEditor))
        {
            ImGui::ShowStyleEditor(&style);
            ImGui::End();
        }
    }

    if (ShowMetricsWindow)
        ImGui::ShowMetricsWindow(&ShowMetricsWindow);

    if (ShowDemoWindow)
        ImGui::ShowDemoWindow(&ShowDemoWindow);
}