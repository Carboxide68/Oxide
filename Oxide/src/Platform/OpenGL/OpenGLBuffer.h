#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Buffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Oxide {

    class OpenGLVertexBuffer : public VertexBuffer {

    public:

        OpenGLVertexBuffer();
        ~OpenGLVertexBuffer();

        void Bind() override;

        void AssociateIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
        const Ref<IndexBuffer>& GetAssociatedIndexBuffer() const override;

        OxideError DrawElements(int count = -1) override; //If count is negative, it will draw the maximum number of indices.
        OxideError DrawArrays(int count = -1) override; //If count is negative, it will draw the maximum number of vertices.

        OxideError BufferData(const size_t size, void* data) override;
        OxideError AppendData(const size_t size, void* data) override;
        OxideError Allocate(const size_t size) override;

        size_t GetBufferSize() const override;

    private:

        virtual void OnBufferLayoutChange();

        Ref<OpenGLVertexArray> m_VAO;
        uint32_t m_RendererID;
        Ref<IndexBuffer> m_IndexBuffer;

        size_t m_BufferSize;
        size_t m_BufferPosition;
        size_t m_BufferStride;
        
    };

    class OpenGLIndexBuffer : public IndexBuffer {

    public:
        OpenGLIndexBuffer();
        ~OpenGLIndexBuffer();

        void Bind() override;

        OxideError BufferData(const size_t size, const void *data) override;
        OxideError AppendData(const size_t size, void* data) override;
        OxideError Allocate(const size_t size) override;

        const size_t& GetBufferSize() override;

    private:

        uint32_t m_RendererID;
        size_t m_BufferSize;
        size_t m_BufferPosition;

    };

}