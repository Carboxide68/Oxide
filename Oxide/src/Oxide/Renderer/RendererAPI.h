#pragma once
#include "Oxide/Core/Base.h"
#include <glm/vec4.hpp>

namespace Oxide {

    class RendererAPI {
    
    public:
        enum class API {
            None = 0,
            OpenGL = 1,
        };

        virtual ~RendererAPI() = default;

        virtual void Init() = 0;
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t widht, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;

        virtual void ClearBuffers() = 0;

        inline static API GetAPI() {return s_API;}
        static Scope<RendererAPI> Create();


    private:

        static API s_API; 

    };
}