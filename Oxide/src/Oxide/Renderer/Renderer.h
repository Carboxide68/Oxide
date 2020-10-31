#pragma once
#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/RendererAPI.h"

//This is supposed to link the window to a rendering context

namespace Oxide {

    class Renderer {
    public:

        enum RenderSettings {

            DRAW_MODE = 0,      //0 = FILL,     1 = WireFrame
            FACE_CULLING = 1,   //0 = FALSE,    1 = TRUE
            ACTIVE_MOUSE = 2,   //0 = FALSE,    1 = TRUE

        };

        struct Viewport {
            uint32_t x;
            uint32_t y;
            uint32_t width;
            uint32_t height;
        };

        Renderer();

        void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
        void SetViewport(uint32_t width, uint32_t height);

        inline const Viewport& GetViewport() const {return m_Viewport;}

        void SetClearColor(const float& r, const float& g, const float& b, const float& a);
        void SetClearColor(const glm::vec4& color);

    protected:

        friend class Window;

        void Init();

        void BeginFrame();
        void EndFrame();

        inline static RendererAPI::API GetAPI() {return RendererAPI::GetAPI();}
    
    private:

        Viewport m_Viewport;
        std::array<uint32_t, 3> m_Settings;
        Scope<RendererAPI> m_RendererAPI;

    };

}