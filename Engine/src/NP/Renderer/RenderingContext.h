#pragma once

namespace Engine
{
    class RenderingContext
    {
    public:
        virtual ~RenderingContext() = default;

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
        virtual void Cleanup() = 0;
    };
}
