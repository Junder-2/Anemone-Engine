#include "nppch.h"
#include "LayerStack.h"

Engine::LayerStack::~LayerStack()
{
    for (Layer* layer : _layers)
    {
        layer->OnDetach();
        delete layer;
    }
}

void Engine::LayerStack::PushLayer(Layer* layer)
{
    _layers.emplace(_layers.begin() + _layerInsertIndex, layer);
    _layerInsertIndex++;
}

void Engine::LayerStack::PopLayer(Layer* layer)
{
    auto it = std::find(_layers.begin(), _layers.begin() + _layerInsertIndex, layer);
    if (it != _layers.begin() + _layerInsertIndex)
    {
        layer->OnDetach();
        _layers.erase(it);
        _layerInsertIndex--;
    }
}
