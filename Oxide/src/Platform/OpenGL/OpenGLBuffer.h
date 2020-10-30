#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Buffer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Oxide {

    class OpenGLVertexBuffer : public VertexBuffer {

    public:

        OpenGLVertexBuffer();
        ~OpenGLVertexBuffer();

        virtual void Bind() override;

        virtual void AssociateIndexBuffer(const Ref<IndexBuffer>& indexBuffer) override;
        virtual const Ref<IndexBuffer>& GetAssociatedIndexBuffer() const override;

        virtual OxideError DrawElements(int count) override; //If count is negative, it will draw the maximum number of indices.
        virtual OxideError DrawArrays(int count) override; //If count is negative, it will draw the maximum number of vertices.

        virtual OxideError BufferData(const size_t size, void* data) override;
        virtual OxideError AppendData(const size_t size, void* data) override;
        virtual OxideError Allocate(const size_t size) override;
        virtual size_t GetBufferSize() const override;

    private:

        virtual void OnBufferLayoutChange();

        Ref<OpenGLVertexArray> m_VAO;
        uint32_t m_RendererID;
        Ref<IndexBuffer> m_IndexBuffer;

        size_t m_BufferSize;
        size_t m_BufferPosition;
        size_t m_BufferStride;

        std::vector<BufferElement> m_BufferLayout;

    };

    class OpenGLIndexBuffer : public IndexBuffer {
        

        
    };

}