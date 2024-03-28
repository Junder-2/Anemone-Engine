#pragma once

namespace Engine
{
    class RenderingContext
    {
    public:
        virtual ~RenderingContext() = default;

        virtual void Init();
        virtual void SwapBuffers();
        virtual void Cleanup();
    };
}
