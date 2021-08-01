

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "OpenGLCommon.h"
#include "Oxide/Core/Log.h"

namespace Oxide {


    //Initializing function variables
    OpenGLVertexBuffer::OpenGLVertexBuffer() {

        glGenBuffers(1, &m_RendererID);
        m_VAO = CreateRef<OpenGLVertexArray>();

        m_VAO->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_VAO->Unbind();

        m_IndexBuffer = nullptr;

        m_BufferSize = 0;

    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer() {

        glDeleteBuffers(1, &m_RendererID);

    }

    void OpenGLVertexBuffer::Bind() {

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        if (m_IndexBuffer) m_IndexBuffer->Bind();

    }

    void OpenGLVertexBuffer::AssociateIndexBuffer(const Ref<IndexBuffer> indexBuffer) {

        m_IndexBuffer = indexBuffer;
        m_VAO->Bind();
        Bind();
        m_VAO->Unbind();        


    }

    const Ref<IndexBuffer>& OpenGLVertexBuffer::GetAssociatedIndexBuffer() const {
        return m_IndexBuffer;
    }

    OxideError OpenGLVertexBuffer::DrawElements(int count, const size_t offset, const size_t basevertex) {
        ZoneScopedN("DrawElements")
        ZoneValue(count);
        
        if (m_IndexBuffer == nullptr) {
            return OxideError::Error;
        }

        if (count == -1) {
            count = m_BufferSize/m_BufferStride;
        }

        TracyGpuZone("DrawElements")
        m_VAO->Bind();
        Bind();
        glDrawElementsBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_INT, (size_t*)offset, basevertex);

        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::DrawArrays(int count, const size_t offset) {
        ZoneScopedN("DrawArrays")
        ZoneValue(count);

        if (count == -1) {
            count = m_BufferSize/m_BufferStride;
        }

        TracyPlot("DrawCount", (int64_t)count);
        TracyGpuZone("DrawArrays")
        m_VAO->Bind();
        Bind();
        glDrawArrays(GL_TRIANGLES, offset, count);

        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::BufferData(const size_t size, void* data) {

        Bind();        
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_BufferSize = size;
        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::SubData(const size_t begin, const size_t size, void* data) {
        if (begin + size > m_BufferSize) {
            CO_CORE_ERROR("Tried to write data outside of buffer! Use allocate first!");
            return OxideError::Error;
        }
        Bind();
        long int pos = begin;
        glBufferSubData(GL_ARRAY_BUFFER, pos, size, data);
        return OxideError::OK;
    }

    OxideError OpenGLVertexBuffer::AppendData(const size_t size, void* data) {


        m_VAO->Bind();
        Bind();
        if ((m_BufferPosition + size) > m_BufferSize) {
            Allocate((m_BufferPosition + size) * 1.5);
        }
        long int pos = m_BufferPosition;
        glBufferSubData(GL_ARRAY_BUFFER, pos, size, data);
        m_BufferPosition += size;
        return OxideError::OK;

    }

    OxideError OpenGLVertexBuffer::Allocate(const size_t size) {
        
        ZoneScopedN("GPUAllocation")
        if (size < m_BufferSize) return OxideError::Error;
        if (m_BufferPosition == 0 || m_BufferSize == 0) {
            m_VAO->Bind();
            Bind();
            glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
            m_BufferSize = size;
            OnBufferLayoutChange();
            return OxideError::OK;            
        }
        TracyGpuZone("Allocation")
        glBindBuffer(GL_COPY_READ_BUFFER, m_RendererID);
        uint newBuffer;
        glGenBuffers(1, &newBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, newBuffer);
        glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, m_BufferPosition);
        glDeleteBuffers(1, &m_RendererID);
        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        
        m_RendererID = newBuffer;
        m_BufferSize = size;

        m_VAO->Bind();
        Bind();
        m_VAO->Unbind();
        OnBufferLayoutChange();
        return OxideError::OK;
    }

    size_t OpenGLVertexBuffer::GetBufferSize() const {
        return m_BufferSize;
    }

    void OpenGLVertexBuffer::OnBufferLayoutChange() {
        ZoneScoped
        m_BufferStride = 0;
        size_t offset = 0;
        
        for (size_t bufferElement = 0; bufferElement < m_BufferLayout.size(); bufferElement++) {

            if (m_BufferLayout[bufferElement].type == OxideType::None) continue;
            m_BufferStride += m_BufferLayout[bufferElement].TypeSize * m_BufferLayout[bufferElement].count;

        }

        m_VAO->Bind(); //TODO: Make a global VertexArray for OpenGL
        Bind();

        for (size_t bufferElement = 0; bufferElement < m_BufferLayout.size(); bufferElement++) {
            if (m_BufferLayout[bufferElement].type == OxideType::None) {glDisableVertexAttribArray(bufferElement); continue;}
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

        if ((m_BufferPosition + size) > m_BufferSize) {
            Allocate((m_BufferPosition + size) * 1.5);
        }
        Bind();
        long int pos = m_BufferPosition;
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, pos, size, data);
        m_BufferPosition += size;
        return OxideError::OK;
    }

    OxideError OpenGLIndexBuffer::Allocate(const size_t size) {
        ZoneScopedN("GPUAllocation")
        if (size < m_BufferSize) return OxideError::Error;
        if (m_BufferPosition == 0 || m_BufferSize == 0) {
            Bind();
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
            m_BufferSize = size;
            return OxideError::OK;            
        }
        TracyGpuZone("Allocation")
        glBindBuffer(GL_COPY_READ_BUFFER, m_RendererID);
        uint newBuffer;
        glGenBuffers(1, &newBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, newBuffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, NULL, GL_STATIC_DRAW);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0, 0, m_BufferSize);
        glDeleteBuffers(1, &m_RendererID);
        glBindBuffer(GL_COPY_READ_BUFFER, 0);

        m_RendererID = newBuffer;
        m_BufferSize = size;
        Bind();
        
        return OxideError::OK;
    }

    const size_t& OpenGLIndexBuffer::GetBufferSize() {
        return m_BufferSize;
    }

}