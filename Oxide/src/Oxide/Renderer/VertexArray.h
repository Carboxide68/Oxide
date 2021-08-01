#pragma once

#include "Oxide/Core/Base.h"

namespace Oxide {

    class VertexArray {
    public:

        VertexArray();
        ~VertexArray();

        uint32_t GetHandle() {return m_RendererID};
        
        void Bind();
        void Unbind();

    private:

        uint32_t m_RendererID;

    };

}