#include "Oxide/Renderer/VertexArray.h"
#include "Oxide/Renderer/OpenGLCommon.h"

namespace Oxide {

    VertexArray::VertexArray() {

        glGenVertexArrays(1, &m_RendererID);

    }

    VertexArray::~VertexArray() {

        glDeleteVertexArrays(1, &m_RendererID);

    }

    void VertexArray::Bind() {
        
        glBindVertexArray(m_RendererID);

    }

    void VertexArray::Unbind() {

        glBindVertexArray(0);

    }

}