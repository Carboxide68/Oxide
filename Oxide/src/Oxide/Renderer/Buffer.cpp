

#include "Oxide/Renderer/Buffer.h"
#include "OpenGLCommon.h"
#include "Oxide/Core/Log.h"

namespace Oxide {

    Ref<Buffer> Buffer::Create(GLenum bufferType, GLenum bufferUse) {
        return Ref<Buffer>(new Buffer(bufferType, bufferUse));
    }

    Buffer::Buffer(GLenum bufferType, GLenum bufferUse) {

        m_BufferType = bufferType;
        m_BufferUse = bufferUse;

        glGenBuffers(1, &m_RendererID);
        glBindBuffer(m_BufferType, m_RendererID);

    }

    Buffer::~Buffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    void Buffer::Bind() {
        glBindBuffer(m_BufferType, m_RendererID);
    }

    OxideError Buffer::BufferData(const size_t size, void* data) {

        Bind();        
        glBufferData(m_BufferType, size, data, m_BufferUse);
        m_BufferSize = size;
        return OxideError::OK;

    }

    OxideError Buffer::SubData(const size_t begin, const size_t size, void* data) {
        if (begin + size > m_BufferSize) {
            CO_CORE_ERROR("[Buffer::SubData] Tried to write data outside of buffer! Use Alloc first!");
            return OxideError::Error;
        }
        Bind();
        long int pos = begin;
        glBufferSubData(m_BufferType, pos, size, data);
        return OxideError::OK;
    }

    OxideError Buffer::AppendData(const size_t size, void* data) {

        Bind();
        if ((bufferPosition + size) > m_BufferSize) {
            Alloc((bufferPosition + size) * 1.5);
        }
        long int pos = bufferPosition;
        glBufferSubData(m_BufferType, pos, size, data);
        bufferPosition += size;
        return OxideError::OK;

    }

    OxideError Buffer::Alloc(const size_t size) {
        
        if (size < m_BufferSize) return OxideError::Error;
        if (bufferPosition == 0 || m_BufferSize == 0) {
            Bind();
            glBufferData(m_BufferType, size, nullptr, m_BufferUse);
            m_BufferSize = size;
            return OxideError::OK;            
        }
        TracyGpuZone("Allocation")
        uint newBuffer = 0;
        glGenBuffers(1, &newBuffer);
        glBindBuffer(m_BufferType, newBuffer);
        glBufferData(m_BufferType, size, NULL, m_BufferUse);
        
        glBindBuffer(GL_COPY_READ_BUFFER, m_RendererID);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, m_BufferType, 0, 0, bufferPosition);
        glDeleteBuffers(1, &m_RendererID);
        glBindBuffer(GL_COPY_READ_BUFFER, 0);
        
        m_RendererID = newBuffer;
        m_BufferSize = size;

        return OxideError::OK;
    }

    size_t Buffer::GetBufferSize() const {
        return m_BufferSize;
    }

    OxideError Buffer::CopyData(Ref<Buffer> other, size_t buffer0_offset, size_t buffer1_offset, size_t size) {
        if ((size + buffer0_offset) > m_BufferSize || (size + buffer1_offset) > other->m_BufferSize) {
            CO_CORE_WARN("[Buffer::CopyData] Bytes + offset can't be bigger than either of the buffers!");
            return OxideError::Error;
        }

        TracyGpuZone("Copy")
        glBindBuffer(GL_COPY_READ_BUFFER, m_RendererID);
        glBindBuffer(GL_COPY_WRITE_BUFFER, other->m_RendererID);
        glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, (long int) buffer0_offset, (long int) buffer1_offset, (long int) size);

        return OxideError::OK;
    }

    Ref<VertexBuffer> VertexBuffer::Create() {
        return Ref<VertexBuffer>(new VertexBuffer());
    }
    
    Ref<VertexBuffer> VertexBuffer::Create(uint handle, size_t buffersize) {
        return Ref<VertexBuffer>(new VertexBuffer(handle, buffersize));
    }

    //Initializing function variables
    VertexBuffer::VertexBuffer() : Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW) {

        glGenBuffers(1, &m_RendererID);
        m_VAO = CreateRef<VertexArray>();

        m_VAO->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_VAO->Unbind();

        m_IndexBuffer = nullptr;

        m_BufferSize = 0;

    }

    VertexBuffer::VertexBuffer(uint handle, size_t buffersize) : Buffer(GL_ARRAY_BUFFER, GL_STATIC_DRAW) {

        m_RendererID = handle;
        m_VAO = CreateRef<VertexArray>();

        m_VAO->Bind();
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        m_VAO->Unbind();
        m_IndexBuffer = nullptr;

        m_BufferSize = buffersize;

    }

    VertexBuffer::~VertexBuffer() {

        this->~Buffer();

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

    Ref<IndexBuffer> VertexBuffer::GetAssociatedIndexBuffer() const {
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

    void VertexBuffer::UpdateLayout() {
        ZoneScoped
        size_t stride = 0;
        size_t offset = 0;
        
        for (size_t bufferElement = 0; bufferElement < bufferLayout.size(); bufferElement++) {

            if (bufferLayout[bufferElement].type == OxideType::None) continue;
            stride += bufferLayout[bufferElement].TypeSize * bufferLayout[bufferElement].count;

        }

        m_VAO->Bind(); //TODO: Make a global VertexArray for OpenGL
        Bind();

        for (size_t bufferElement = 0; bufferElement < bufferLayout.size(); bufferElement++) {
            if (bufferLayout[bufferElement].type == OxideType::None) {glDisableVertexAttribArray(bufferElement); continue;}
            glEnableVertexAttribArray(bufferElement);
            glVertexAttribPointer(  bufferElement, bufferLayout[bufferElement].count, 
                                    OpenGLGetType(bufferLayout[bufferElement].type), 
                                    GL_FALSE, stride, (void*)offset);
            
            offset += bufferLayout[bufferElement].TypeSize * bufferLayout[bufferElement].count;
        }
    }

    void VertexBuffer::EnableAttrib(uint attrib) {
        m_VAO->Bind();
        glEnableVertexAttribArray(attrib);
    }

    void VertexBuffer::DisableAttrib(uint attrib) {
        m_VAO->Bind();
        glDisableVertexAttribArray(attrib);
    }


//_____________________________________________________________________________________________________________________________________

    Ref<IndexBuffer> IndexBuffer::Create() {
        return Ref<IndexBuffer>(new IndexBuffer());
    }

    IndexBuffer::IndexBuffer() : Buffer(GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW) {
        glGenBuffers(1, &m_RendererID);
        m_BufferSize = 0;
        bufferPosition = 0;
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }
}