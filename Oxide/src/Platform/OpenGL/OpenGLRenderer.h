#pragma once
#include "Oxide/Renderer/Renderer.h"
#include <glm/vec4.hpp>

namespace Oxide {

    class OpenGLRenderer : public Renderer {
    public:

        OpenGLRenderer();
        ~OpenGLRenderer();

        void SetViewport(Renderer::Viewport viewport) override;
        void SetViewport(uint32_t width, uint32_t height) override;

        void SetClearColor(const float& r, const float& g, const float& b, const float& a) override;

        void ClearBuffers() override;

        void ChangeState(Renderer::RenderSettings setting, bool toggle) override;


    protected:

        void Init() override;

        void BeginFrame() override;
        void EndFrame() override;

    private:

        glm::vec4 m_ClearColor;

    };

}