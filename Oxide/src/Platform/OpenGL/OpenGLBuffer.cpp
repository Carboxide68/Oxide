

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "OpenGLCommon.h"

namespace Oxide {


    //Initializing function variables
    OpenGLVertexBuffer::OpenGLVertexBuffer() {

        m_VAO->Bind();
        glGenBuffers(1, &m_RendererID);

        m_IndexBuffer = nullptr;

        m_BufferSize = 0;

    }

    void OpenGLVertexBuffer::AssociateIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {

        m_IndexBuffer = indexBuffer;

    }

    const Ref<IndexBuffer>& OpenGLVertexBuffer::GetAssociatedIndexBuffer() const {
        return m_IndexBuffer;
    }

    OxideError OpenGLVertexBuffer::BufferData(const size_t size, void* data) {
        
        m_VAO->Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::AppendData(const size_t size, void* data) {

        m_VAO->Bind();
        if (m_BufferPosition + size > m_BufferSize) {
            CO_ASSERT(false, "Buffer not big enough to append data!");
            return OxideError::Error;
        }
        glBufferSubData(GL_ARRAY_BUFFER, m_BufferPosition, size, data);
        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::Allocate(const size_t size) {

        m_VAO->Bind();
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
        m_BufferSize = size;
        return OxideError::OK;
        
    }

    size_t OpenGLVertexBuffer::GetBufferSize() const {
        return m_BufferSize;
    }

    void OpenGLVertexBuffer::OnBufferLayoutChange() {

        size_t stride;
        size_t offset;
        
        for (int bufferElement = 0; bufferElement < m_BufferLayout.size(); bufferElement++) {

            stride += m_BufferLayout[bufferElement].TypeSize * m_BufferLayout[bufferElement].Count;

        }

        m_VAO->Bind(); //Make a global VertexArray for OpenGL
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

        for (int bufferElement = 0; bufferElement < m_BufferLayout.size(); bufferElement++) {
            
            glVertexAttribPointer(  bufferElement, m_BufferLayout[bufferElement].Count, 
                                    OpenGLGetType(m_BufferLayout[bufferElement].type), GL_FALSE, stride, &offset);
            
            offset += m_BufferLayout[bufferElement].TypeSize * m_BufferLayout[bufferElement].Count;
            
        }

    }

    

}