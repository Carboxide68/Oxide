

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

    void OpenGLVertexBuffer::AssociateIndexBuffer(const Ref<IndexBuffer> indexBuffer) {

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
        if (m_BufferStride != 0) {
            if ((size_t)count > m_BufferSize/m_BufferStride) {
                CO_CORE_ERROR("Count exceeds number of vertices!");
                return OxideError::Error;
            }
        } else {
            return OxideError::Error;
        }

        if (count == -1) {
            count = m_BufferSize/m_BufferStride;
        }

        m_VAO->Bind();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_SHORT, NULL);

        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::DrawArrays(int count) {

        if (m_BufferStride != 0) {
            if ((size_t)count > m_BufferSize/m_BufferStride) {
                CO_CORE_ERROR("Count exceeds number of vertices!");
                return OxideError::Error;
            }
        } else {
            CO_CORE_ERROR("Can't draw empty buffer!");
            return OxideError::Error;
        }

        if (count == -1) {
            count = m_BufferSize/m_BufferStride;
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
            CO_CORE_ERROR("Buffer not big enough to append data!");
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
        size_t offset = 0;
        
        for (size_t bufferElement = 0; bufferElement < m_BufferLayout.size(); bufferElement++) {

            m_BufferStride += m_BufferLayout[bufferElement].TypeSize * m_BufferLayout[bufferElement].count;

        }

        m_VAO->Bind(); //TODO: Make a global VertexArray for OpenGL
        Bind();

        for (size_t bufferElement = 0; bufferElement < m_BufferLayout.size(); bufferElement++) {
            if (m_BufferLayout[bufferElement].type == OxideType::None) continue;
            glEnableVertexAttribArray(bufferElement);
            glVertexAttribPointer(  bufferElement, m_BufferLayout[bufferElement].count, 
                                    OpenGLGetType(m_BufferLayout[bufferElement].type), 
                                    GL_FALSE, m_BufferStride, (void*)offset);
            
            offset += m_BufferLayout[bufferElement].TypeSize * m_BufferLayout[bufferElement].count;
        }
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer() {
        glGenBuffers(1, &m_RendererID);
        m_BufferSize = 0;
        m_BufferPosition = 0;
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    void OpenGLIndexBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    OxideError OpenGLIndexBuffer::BufferData(const size_t size, const void *data) {

        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_BufferSize = size;
        m_BufferPosition = size;
        return OxideError::OK;

    }

    OxideError OpenGLIndexBuffer::AppendData(const size_t size, void* data) {

        if (m_BufferPosition + size > m_BufferSize) {
            CO_ERROR("Buffer too small!");
            return OxideError::Error;
        }
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, m_BufferPosition, size, data);
        m_BufferPosition = size + m_BufferPosition;
        return OxideError::OK;
    }

    OxideError OpenGLIndexBuffer::Allocate(const size_t size) {
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
        m_BufferSize = size;
        return OxideError::OK;
    }

    const size_t& OpenGLIndexBuffer::GetBufferSize() {
        return m_BufferSize;
    }

}