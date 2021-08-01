#pragma once
#include "Oxide/Renderer/OpenGLCommon.h"
#include <glm/vec4.hpp>

namespace Oxide {

    class CRenderer  {
    public:

        ~CRenderer();

        void SetViewport(CRenderer::Viewport viewport);
        void SetViewport(uint32_t width, uint32_t height);

        void SetClearColor(const float& r, const float& g, const float& b, const float& a);

        void ClearBuffers();

        /**@brief Direct port of glEnable*/
        void Enable(GLenum capability) {glEnable(capability);}

        /**@brief Direct port of glDisable*/
        void Disable(GLenum capability) {glDisable(capability);};
        /**@brief Direct port of glGet*/
        bool GetCapability(GLenum capability) {return glGet(capability);}

        static Ref<CRenderer> Create();

    private:
        CRenderer();

        void Init();

        void BeginFrame();
        void EndFrame();

        glm::vec4 m_ClearColor;

    };
}