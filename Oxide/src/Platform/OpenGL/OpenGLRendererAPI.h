#pragma once
#include "Oxide/Renderer/RendererAPI.h"
#include <glm/vec4.hpp>

namespace Oxide {

    class OpenGLRendererAPI : public RendererAPI {
    public:

        OpenGLRendererAPI();
        ~OpenGLRendererAPI();

        virtual void Init();
        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t widht, uint32_t height);
        virtual void SetClearColor(const glm::vec4& color);

        virtual void ClearBuffers();

    private:

        glm::vec4 m_ClearColor;

    };

}