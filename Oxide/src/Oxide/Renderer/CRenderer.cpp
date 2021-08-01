#include "Oxide/Renderer/CRenderer.h"
#include "Oxide/Core/Log.h"
#include <GL/glew.h>

namespace Oxide {

    void onglerror(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userparam) {
        if (severity == GL_DEBUG_SEVERITY_HIGH) {
            CO_CORE_CRITICAL("OpenGL Error! Severity: High | Description: %s", message);
        }
    }

    CRenderer::~CRenderer() = default;

    CRenderer::CRenderer() : m_ClearColor(1, 1, 1, 1) {

    }

    void CRenderer::Init() {

        {
            GLenum err = glewInit();
            CO_CORE_ASSERT(err != GLEW_OK, "Something went wrong when initializing glew! Error: %s", glewGetErrorstring(err));
        }
        TracyGpuContext
        glDebugMessageCallback(onglerror, NULL);

    }

    void CRenderer::SetViewport(CRenderer::Viewport viewport) {

        glViewport(viewport.x, viewport.y, viewport.width, viewport.height);

    }

    void CRenderer::SetViewport(uint32_t width, uint32_t height) { 
        glViewport(0, 0, width, height);
    }

    void CRenderer::SetClearColor(const float& r, const float& g, const float& b, const float& a) {
        glClearColor(r, g, b, a);
    }

    void CRenderer::ClearBuffers() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

    void CRenderer::BeginFrame() {
        ClearBuffers();
    }

    void CRenderer::EndFrame() {
        TracyGpuCollect
    }

}