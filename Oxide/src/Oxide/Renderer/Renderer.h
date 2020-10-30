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

        Renderer();

        void BeginFrame();
        void EndFrame();

        inline static RendererAPI::API GetAPI() {return RendererAPI::GetAPI();}
    
    private:

        std::array<uint32_t, 3> m_Settings;
        Scope<RendererAPI> m_RendererAPI;

    };

}