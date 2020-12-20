

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "OpenGLCommon.h"
#include "Oxide/Core/Log.h"

namespace Oxide {


    //Initializing function variables
    OpenGLVertexBuffer::OpenGLVertexBuffer() {

        glGenBuffers(1, &m_RendererID);

        m_VAO = CreateRef<OpenGLVertexArray>();

        m_IndexBuffer = nullptr;

        m_BufferSize = 0;

    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {

        glDeleteBuffers(1, &m_RendererID);

    }

    void OpenGLVertexBuffer::Bind() {

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

    }

    void OpenGLVertexBuffer::AssociateIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {

        m_IndexBuffer = indexBuffer;
        m_VAO->Bind();
        m_IndexBuffer->Bind();

    }

    const Ref<IndexBuffer>& OpenGLVertexBuffer::GetAssociatedIndexBuffer() const {
        return m_IndexBuffer;
    }

    OxideError OpenGLVertexBuffer::DrawElements(int count) {

        if (m_IndexBuffer == nullptr) {
            return OxideError::Error;
        }

        m_VAO->Bind();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL);

        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::DrawArrays(int count) {

        if (m_BufferStride != 0) {
            if ((size_t)count > m_BufferSize/m_BufferStride) {
                CO_ERROR(false, "Count exceeds number of vertexes!");
                return OxideError::Error;
            }
        }

        m_VAO->Bind();
        Bind();
        glDrawArrays(GL_TRIANGLES, 0, count);

        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::BufferData(const size_t size, void* data) {

        Bind();        
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_BufferSize = size;
        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::AppendData(const size_t size, void* data) {

        m_VAO->Bind();
        if (m_BufferPosition + size > m_BufferSize) {
            CO_ERROR(false, "Buffer not big enough to append data!");
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

        m_BufferStride = 0;
        int offset = 0;
        
        for (size_t bufferElement = 0; bufferElement < m_BufferLayout.size(); bufferElement++) {

            m_BufferStride += m_BufferLayout[bufferElement].TypeSize * m_BufferLayout[bufferElement].Count;

        }

        m_VAO->Bind(); //TODO: Make a global VertexArray for OpenGL
        Bind();

        for (size_t bufferElement = 0; bufferElement < m_BufferLayout.size(); bufferElement++) {
            glEnableVertexAttribArray(bufferElement);
            glVertexAttribPointer(  bufferElement, m_BufferLayout[bufferElement].Count, 
                                    OpenGLGetType(m_BufferLayout[bufferElement].type), 
                                    GL_FALSE, m_BufferStride, (void*)offset);
            
            offset += m_BufferLayout[bufferElement].TypeSize * m_BufferLayout[bufferElement].Count;
        }
    }
}