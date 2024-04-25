#include "anepch.h"
#include "ImGuiLayer.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "ANE/Input/EditorInputsystem.h"
#include "ANE/Input/Input.h"
#include "ANE/Input/InputAction.h"

namespace Engine
{
    //std::queue<std::string_view> Logger::Texts;

    ImGuiLayer::ImGuiLayer(const char* layerName): Layer(layerName)
    {
        //EnableFlag("MainMenuWindow");
        //EnableFlag("LogWindow");
        //EnableFlag("SceneViewWindow");
        //EnableFlag("HierarchyWindow");
        GetEditorInputSystem().BindKeyboardInput(KeyCodeB, MakeDelegate(this, &ImGuiLayer::AddDebugMessage));
        // GetInputSystem().BindKeyboardInput(KeyCodeB, MakeDelegate(this, &ImGuiLayer::AddDebugMessage));
    }

    void ImGuiLayer::AddDebugMessage(InputValue value)
    {
        //ANE_ELOG("We are pressing five");
        //std::string_view hey = "yo";
        //Logger::SubmitText(hey);
        ANE_ELOG("Hey");
    }

    ImGuiLayer::~ImGuiLayer()
    {
    }

    void ImGuiLayer::OnAttach()
    {
    }

    void ImGuiLayer::OnDetach()
    {
    }

    void ImGuiLayer::OnUpdate(float deltaTime)
    {
        // AddDebugMessage();
    }

    void ImGuiLayer::OnEvent(Event& e)
    {
    }


    ImGuiLayer* ImGuiLayer::Create(const char* str)
    {
        return new ImGuiLayer(str);

    }


    void ImGuiLayer::OnUIRender()
    {
        bool open = true;
        DockSpace(&open);

        // ImGui::ShowDemoWindow()

        //if (GetFlag("MainMenuWindow")) MainMenuWindow();
        //if (GetFlag("FileMenuWindow")) FileMenuWindow();
        //if (GetFlag("CreateSceneWindow")) CreateSceneWindow();
        //if (GetFlag("LogWindow")) LogWindow(&open);
        //if (GetFlag("SceneViewWindow")) SceneViewWindow();
        //if (GetFlag("HierarchyWindow")) HierarchyWindow();
        //if (GetFlag("ShowDebugOverlay")) ShowInputDebugOverlay();
    }

    void ImGuiLayer::DockSpace(bool* open)
    {
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus; /*| ImGuiWindowFlags_no;*/
        }
        else dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding) ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace", open, window_flags);

        if (!opt_padding) ImGui::PopStyleVar();

        if (opt_fullscreen) ImGui::PopStyleVar(2);

        ImGuiIO& io = ImGui::GetIO();

        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

        ImGui::End();
    }

    void ImGuiLayer::ShowInputDebugOverlay()
    {
        bool open = true;
        const InputSystem& inputManager = GetInputSystem();
        static int location = 0;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoDocking;
        if (location >= 0)
        {
            const float PAD = 10.0f;
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = viewport->WorkSize;
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
            window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
            window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
            window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            window_flags |= ImGuiWindowFlags_NoMove;
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Example: Simple overlay", &open, window_flags))
        {
            const Vector2 mousePos = inputManager.GetMousePos();
            ImGui::Text("Mouse Pos: (%.3f,%.3f)", mousePos.X, mousePos.Y);
            const MouseButtonValues mouseButtonValues = inputManager.GetMouseButtonValues();
            ImGui::Text("Mouse Buttons: (");
            bool start = true;
            for (int i = 0; i < MOUSE_BUTTON_MAX; i++)
            {
                const int buttonState = mouseButtonValues.GetTriggerState(i);
                if (buttonState == 0) continue;
                ImGui::SameLine(0, start ? .0f : -1.0f);
                ImGui::Text("%d:%d", i, buttonState);
                start = false;
            }
            ImGui::SameLine(0, 0);
            ImGui::Text(")");

            const auto keyValues = inputManager.GetCurrentTriggeredKeys();
            ImGui::Text("Keyboard: (");
            start = true;
            for (auto keyValue : keyValues)
            {
                if (keyValue.GetIntValue() == 0) continue;
                ImGui::SameLine(0, start ? .0f : -1.0f);
                ImGui::Text("%d:%d", keyValue.GetBindingId(), keyValue.GetIntValue());
                start = false;
            }
            ImGui::SameLine(0, 0);
            ImGui::Text(")");
            ImGui::End();
        }
    }

    void ImGuiLayer::MainMenuWindow()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Create Scene")) EnableFlag("CreateSceneWindow");

                if (ImGui::MenuItem("Open Scene"))
                {
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Debug"))
            {
                if (ImGui::MenuItem("Input Analysis")) ToggleFlag("ShowDebugOverlay");
                if (ImGui::MenuItem("Performance Analysis"))
                {
                    // Todo: menu
                    // ToggleFlag("ShowDebugOverlay");
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Scene Window")) EnableFlag("SceneViewWindow");
                if (ImGui::MenuItem("Log Window")) EnableFlag("LogWindow");
                if (ImGui::MenuItem("Hierarchy Window")) EnableFlag("HierarchyWindow");

                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }
    }

    void ImGuiLayer::LogWindow(bool* open)
    {
        // static Logger log;
        //
        // ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        //
         ImGui::Begin("Log", open);
        //
        // if (std::string_view textToPrint; Logger::GetText(textToPrint))log.AddLog(std::basic_string<char>(textToPrint).c_str());
        //
         ImGui::End();
        //
        // log.Draw("Log", open);
    }

    void ImGuiLayer::SceneViewWindow()
    {
        static ImGuiWindowFlags windowflags = ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("Scene View", nullptr, windowflags);
        ImGui::End();
    }

    void ImGuiLayer::HierarchyWindow()
    {
        static ImGuiWindowFlags windowflags = ImGuiWindowFlags_NoCollapse;
        ImGui::Begin("HierarchyWindow", nullptr, windowflags);

        ImGui::End();
    }

    void ImGuiLayer::FileMenuWindow()
    {
        if (ImGui::BeginMenu("manu"))
        {
            if (ImGui::MenuItem("CreateScene"))
            {
                EnableFlag("CreateSceneWindow");
                DisableFlag("FileMenuWindow");
            }

            ImGui::EndMenu();
        }
    }

    void ImGuiLayer::CreateSceneWindow()
    {
        if (ImGui::Begin("Create Scene"))
        {
            // this one crashes when minimized
        }
        ImGui::End();
    }

    bool ImGuiLayer::GetFlag(const char* key)
    {
        if (_menuFlags.contains(key)) return _menuFlags[key]; // should work.
        _menuFlags.emplace(std::make_pair(key, false));
        return false;
    }

    void ImGuiLayer::DisableFlag(const char* key)
    {
        if (_menuFlags.contains(key)) _menuFlags[key] = false;
        else _menuFlags.emplace(std::make_pair(key, false));
    }

    void ImGuiLayer::EnableFlag(const char* key)
    {
        if (_menuFlags.contains(key)) _menuFlags[key] = true;
        else _menuFlags.emplace(std::make_pair(key, true));
    }

    void ImGuiLayer::ToggleFlag(const char* key)
    {
        if (_menuFlags.contains(key)) _menuFlags[key] = !_menuFlags[key];
        else _menuFlags.emplace(std::make_pair(key, false));
    }
}
