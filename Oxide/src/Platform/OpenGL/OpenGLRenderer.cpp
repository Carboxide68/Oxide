#include "Platform/OpenGL/OpenGLRenderer.h"
#include "Oxide/Core/Log.h"
#include <GL/glew.h>

namespace Oxide {

    OpenGLRenderer::~OpenGLRenderer() = default;

    OpenGLRenderer::OpenGLRenderer() : m_ClearColor(1, 1, 1, 1) {

    }

    void OpenGLRenderer::Init() {

        {
            GLenum err = glewInit();
            CO_CORE_ASSERT(err != GLEW_OK, "Something went wrong when initializing glew! Error: %s", glewGetErrorstring(err));
        }

    }

    void OpenGLRenderer::SetViewport(CRenderer::Viewport viewport) {

        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);

    }

    void OpenGLRenderer::SetViewport(uint32_t width, uint32_t height) { 
        glViewport(0, 0, width, height);
    }

    void OpenGLRenderer::SetClearColor(const float& r, const float& g, const float& b, const float& a) {
        glClearColor(r, g, b, a);
    }

    void OpenGLRenderer::ClearBuffers() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void OpenGLRenderer::BeginFrame() {
        ClearBuffers();
    }

    void OpenGLRenderer::EndFrame() {

    }

    void OpenGLRenderer::ChangeState(CRenderer::RenderSettings setting, bool toggle) {

        switch (setting) {
            case CRenderer::RenderSettings::BLEND:

                if (toggle)
                    glEnable(GL_BLEND);
                else
                    glDisable(GL_BLEND);
                break;

            case CRenderer::RenderSettings::DEPTH_TEST:

                if (toggle)
                    glEnable(GL_DEPTH_TEST);
                else
                    glDisable(GL_DEPTH_TEST);
                break;
            case CRenderer::RenderSettings::FACE_CULLING:

                if (toggle)
                    glEnable(GL_CULL_FACE);
                else
                    glDisable(GL_CULL_FACE);
                break;
            case CRenderer::RenderSettings::STENCIL_TEST:

                if (toggle)
                    glEnable(GL_STENCIL_TEST);
                else
                    glDisable(GL_STENCIL_TEST);
                break;
            
            default:
                CO_ERROR("Renderer Setting doesn't exist!");
                break;
        }

    }

}