#pragma once

#include "Oxide/Core/Base.h"

namespace Oxide {

    class OpenGLVertexArray {
    public:

        OpenGLVertexArray();
        ~OpenGLVertexArray();
        
        void Bind();
        void Unbind();

    private:

        uint32_t m_RendererID;

    };

}