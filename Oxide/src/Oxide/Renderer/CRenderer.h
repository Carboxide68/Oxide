#pragma once
#include "Oxide/Renderer/OpenGLCommon.h"
#include <glm/vec4.hpp>

namespace Oxide {

    struct Viewport {
        uint32_t x;
        uint32_t y;
        uint32_t width;
        uint32_t height;
    };

    class CRenderer  {
    public:

        ~CRenderer();

        inline ulong GetFrame() {return m_Frame;}

        void SetViewport(Viewport viewport);
        void SetViewport(uint32_t width, uint32_t height);

        void SetClearColor(const float& r, const float& g, const float& b, const float& a);

        void ClearBuffers();

        /**@brief Direct port of glEnable*/
        void Enable(GLenum capability);

        /**@brief Direct port of glDisable*/
        void Disable(GLenum capability);

        static Scope<CRenderer> Create();

    private:
        CRenderer();

        void Init();

        void BeginFrame();
        void EndFrame();

        friend class Window;

        ulong m_Frame = 0;
        glm::vec4 m_ClearColor;

    };
}