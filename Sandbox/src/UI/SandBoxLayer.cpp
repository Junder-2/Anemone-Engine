#include "SandBoxLayer.h"

SandBox::SandBoxLayer::~SandBoxLayer()
{

}

void SandBox::SandBoxLayer::Init()
{
}

void SandBox::SandBoxLayer::OnDetach()
{

}

void SandBox::SandBoxLayer::OnUpdate(float deltaTime)
{
    Layer::OnUpdate(deltaTime);
    if (_activeScene) _activeScene->OnUpdate(deltaTime);

}

void SandBox::SandBoxLayer::OnEvent(Engine::Event& e)
{

}

void SandBox::SandBoxLayer::OnUIRender()
{

}

void SandBox::SandBoxLayer::OnAttach()
{

}
