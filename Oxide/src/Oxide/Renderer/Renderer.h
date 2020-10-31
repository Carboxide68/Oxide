#pragma once
#include "Oxide/Core/Base.h"
#include <glm/vec4.hpp>

namespace Oxide {

    class Renderer {
    
    public:
        enum class API {
            None = 0,
            OpenGL = 1,
        };

        enum RenderSettings {

            FACE_CULLING,   //0 = FALSE,    1 = TRUE
            DEPTH_TEST,
            BLEND,
            STENCIL_TEST

        };

        struct Viewport {
            uint32_t x;
            uint32_t y;
            uint32_t width;
            uint32_t height;
        };

        virtual ~Renderer() = default;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
        virtual void SetClearColor(const glm::vec4& color) = 0;

        virtual void ClearBuffers() = 0;

        virtual void ChangeState(RenderSettings setting, bool toggle) = 0;

        inline static API GetAPI() {return s_API;}
        static Scope<Renderer> Create();

    protected:

        friend class Window;

        virtual void Init() = 0;

        virtual void BeginFrame() = 0;
        virtual void EndFrame() = 0;

    private:
    
        Viewport m_Viewport;
        static API s_API; 

    };
}