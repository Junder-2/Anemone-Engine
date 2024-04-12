#pragma once
#include "Layer.h"
#include "ANE/Utilities/ImGuiUtilities.h"

namespace Engine
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer(const char* layerName);
        ~ImGuiLayer() override;
        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnEvent(Event& e) override;
        void OnUIRender() override;

    private:
        ImGuiUtilities::ImGuiDockNodeFlags _flags;
    };
}
