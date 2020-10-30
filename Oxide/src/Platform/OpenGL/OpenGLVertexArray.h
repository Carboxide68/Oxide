#pragma once

#include "Oxide/Core/Base.h"

namespace Oxide {

    class OpenGLVertexArray {
    public:

        OpenGLVertexArray();
        ~OpenGLVertexArray();
        
        void Bind();

    private:

        uint32_t m_RendererID;

    };

}