#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include <GL/glew.h>

namespace Oxide {

    OpenGLRendererAPI::~OpenGLRendererAPI() = default;

    OpenGLRendererAPI::OpenGLRendererAPI() : m_ClearColor(1, 1, 1, 1) {

    }

    void OpenGLRendererAPI::Init() {

        {
            GLenum err = glewInit();
            CO_CORE_ASSERT(err != GLEW_OK, "Something went wrong when initializing glew! Error: %s", glewGetErrorstring(err));
        }

    }

    void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

        glViewport(x, y, width, height);

    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
        glClearColor(color.x, color.y, color.z, color.w);
    }

    void OpenGLRendererAPI::ClearBuffers() {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }

}