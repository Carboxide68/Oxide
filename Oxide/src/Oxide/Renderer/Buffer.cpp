

#include "Oxide/Renderer/Buffer.h"
#include "OpenGLCommon.h"
#include "Oxide/Core/Log.h"

namespace Oxide {


    //Initializing function variables
    VertexBuffer::VertexBuffer() {

        glGenBuffers(1, &m_RendererID);
        m_VAO = CreateRef<OpenGLVertexArray>();

        m_VAO->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_VAO->Unbind();

        m_IndexBuffer = nullptr;

        m_BufferSize = 0;

    }

    VertexBuffer::~VertexBuffer() {

        glDeleteBuffers(1, &m_RendererID);

    }

    void VertexBuffer::Bind() {

        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        if (m_IndexBuffer) m_IndexBuffer->Bind();

    }

    void VertexBuffer::AssociateIndexBuffer(const Ref<IndexBuffer> indexBuffer) {

        m_IndexBuffer = indexBuffer;
        m_VAO->Bind();
        Bind();
        m_VAO->Unbind();        


    }

    const Ref<IndexBuffer>& VertexBuffer::GetAssociatedIndexBuffer() const {
        return m_IndexBuffer;
    }

    OxideError VertexBuffer::DrawElements(const size_t count, const size_t offset, const size_t basevertex) {
        ZoneScopedN("DrawElements")
        ZoneValue(count);
        
        if (m_IndexBuffer == nullptr) {
            return OxideError::Error;
        }

        TracyGpuZone("DrawElements")
        m_VAO->Bind();
        Bind();
        glDrawElementsBaseVertex(GL_TRIANGLES, count, GL_UNSIGNED_INT, (size_t*)offset, basevertex);

        return OxideError::OK;

    }

    OxideError VertexBuffer::DrawArrays(const size_t count, const size_t offset) {
        ZoneScopedN("DrawArrays")
        ZoneValue(count);

        TracyPlot("DrawCount", (int64_t)count);
        TracyGpuZone("DrawArrays")
        m_VAO->Bind();
        Bind();
        glDrawArrays(GL_TRIANGLES, offset, count);

        return OxideError::OK;

    }

    OxideError VertexBuffer::BufferData(const size_t size, void* data) {

        Bind();        
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_BufferSize = size;
        return OxideError::OK;

    }

    OxideError VertexBuffer::SubData(const size_t begin, const size_t size, void* data) {
        if (begin + size > m_BufferSize) {
            CO_CORE_ERROR("Tried to write data outside of buffer! Use Alloc first!");
            return OxideError::Error;
        }
        Bind();
        long int pos = begin;
        glBufferSubData(GL_ARRAY_BUFFER, pos, size, data);
        return OxideError::OK;
    }

    OxideError VertexBuffer::AppendData(const size_t size, void* data) {


        Bind();
        if ((m_BufferPosition + size) > m_BufferSize) {
            Alloc((m_BufferPosition + size) * 1.5);
        }
        long int pos = m_BufferPosition;
        glBufferSubData(GL_ARRAY_BUFFER, pos, size, data);
        m_BufferPosition += size;
        return OxideError::OK;

    }

    OxideError VertexBuffer::Alloc(const size_t size) {
        
        ZoneScopedN("GPUAllocation")
        if (size < m_BufferSize) return OxideError::Error;
        if (m_BufferPosition == 0 || m_BufferSize == 0) {
            m_VAO->Bind();
            Bind();
            glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
            m_BufferSize = size;
            UpdateLayout();
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
        UpdateLayout();
        return OxideError::OK;
    }

    size_t VertexBuffer::GetBufferSize() const {
        return m_BufferSize;
    }

    void VertexBuffer::UpdateLayout() {
        ZoneScoped
        size_t stride = 0;
        size_t offset = 0;
        
        for (size_t bufferElement = 0; bufferElement < BufferLayout.size(); bufferElement++) {

            if (BufferLayout[bufferElement].type == OxideType::None) continue;
            stride += BufferLayout[bufferElement].TypeSize * BufferLayout[bufferElement].count;

        }

        m_VAO->Bind(); //TODO: Make a global VertexArray for OpenGL
        Bind();

        for (size_t bufferElement = 0; bufferElement < BufferLayout.size(); bufferElement++) {
            if (BufferLayout[bufferElement].type == OxideType::None) {glDisableVertexAttribArray(bufferElement); continue;}
            glEnableVertexAttribArray(bufferElement);
            glVertexAttribPointer(  bufferElement, BufferLayout[bufferElement].count, 
                                    OpenGLGetType(BufferLayout[bufferElement].type), 
                                    GL_FALSE, stride, (void*)offset);
            
            offset += BufferLayout[bufferElement].TypeSize * BufferLayout[bufferElement].count;
        }
    }

    Ref<VertexBuffer> VertexBuffer::Create() {
        return CreateRef(new VertexBuffer());
    }

    IndexBuffer::IndexBuffer() {
        glGenBuffers(1, &m_RendererID);
        m_BufferSize = 0;
        m_BufferPosition = 0;
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    void IndexBuffer::Bind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    OxideError IndexBuffer::BufferData(const size_t size, const void *data) {

        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
        m_BufferSize = size;
        m_BufferPosition = size;
        return OxideError::OK;

    }

    OxideError IndexBuffer::AppendData(const size_t size, void* data) {

        if ((m_BufferPosition + size) > m_BufferSize) {
            Alloc((m_BufferPosition + size) * 1.5);
        }
        Bind();
        long int pos = m_BufferPosition;
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, pos, size, data);
        m_BufferPosition += size;
        return OxideError::OK;
    }

    OxideError IndexBuffer::Alloc(const size_t size) {
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

    const size_t& IndexBuffer::GetBufferSize() {
        return m_BufferSize;
    }


    Ref<IndexBuffer> IndexBuffer::Create() {
        return CreateRef(new IndexBuffer());
    }

}