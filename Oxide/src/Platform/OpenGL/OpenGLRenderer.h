#pragma once
#include "Oxide/Renderer/Renderer.h"
#include <glm/vec4.hpp>

namespace Oxide {

    class OpenGLRenderer : public Renderer {
    public:

        OpenGLRenderer();
        ~OpenGLRenderer();

        void Init() override;
        void SetViewport(uint32_t x, uint32_t y, uint32_t widht, uint32_t height) override;
        void SetClearColor(const glm::vec4& color) override;

        void ClearBuffers() override;

        void ChangeState(Renderer::RenderSettings setting, bool toggle) override;


    private:

        glm::vec4 m_ClearColor;

    };

}