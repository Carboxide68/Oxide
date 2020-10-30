#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL/OpenGLCommon.h"

namespace Oxide {

    OpenGLVertexArray::OpenGLVertexArray() {

        glGenVertexArrays(1, &m_RendererID);

    }

    OpenGLVertexArray::~OpenGLVertexArray() {

        glDeleteVertexArrays(1, &m_RendererID);

    }

    void OpenGLVertexArray::Bind() {
        
        glBindVertexArray(m_RendererID);

    }

}