#include "anepch.h"
#include "EditorLayer.h"
#include <imgui.h>

#include "ANE/Core/Entity/Entity.h"
#include "LayerStack.h"
#include "ANE/Events/EventHandler.h"

//Temp includes. Can probably change how these components are referenced. Maybe a scene manager hsould be in charge of that
//kind of thing
#include "ANE/Core/Scene/Components/RenderComponent.h"

Engine::EditorLayer::EditorLayer(const std::string& name) : Layer(name)
{
}

Engine::EditorLayer::~EditorLayer()
{


}

void Engine::EditorLayer::OnAttach()
{

    // You would have a "Read from config files to find correct panel layout" method here

    // Then you would call load methods to load the most recent project

    //Then you would load the scene from the file path listed from that project

    CreateTestScene();

}

void Engine::EditorLayer::OnEvent(Event& e)
{
    EventHandler::DispatchEditorEvents();
    EventHandler::DispatchAppEvents();
}

void Engine::EditorLayer::OnUIRender()
{
    ImGui::Begin("Hello");
    ImGui::Button("Button");
    ImGui::End();

    ImGui::ShowDemoWindow();
}

void Engine::EditorLayer::OnUpdate(float deltaTime)
{
    if (_activeScene) _activeScene->OnUpdate(deltaTime);
}

void Engine::EditorLayer::OnEventTest(Event& e)
{
    ANE_LOG_INFO("Hello {0}?: {1}", _debugName, e.GetEventCategories());
}

void Engine::EditorLayer::CreateTestScene()
{
    //Add scene to layer
    AddScene<Scene>("Game");

    //Create a Entity
    Entity ent = GetActiveScene()->Create("Square Entity");

    //Add component to entity
    ent.AddComponent<RenderComponent>();
    // ent.AddComponent<NativeScriptComponent>().Bind<CameraController>();

    //Get Component from entity
    if (RenderComponent comp; ent.TryGetComponent<RenderComponent>(comp))
    {
        TagComponent tag;
        ent.TryGetComponent(tag);
        ANE_ENGINE_LOG_WARN("We have a renderComponent with tag: {0} on entity: {1}", comp.ToString(), tag.Tag);
    }
}



