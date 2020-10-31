#include "Platform/OpenGL/OpenGLRenderer.h"
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

    void OpenGLRenderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

        glViewport(x, y, width, height);

    }

    void OpenGLRenderer::SetClearColor(const glm::vec4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void OpenGLRenderer::ClearBuffers() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

}