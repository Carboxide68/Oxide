#pragma once

#include "Oxide/Core/Base.h"
#include <vector>

class VertexBuffer;

namespace Oxide {

    class IndexBuffer {
    public:
        virtual ~IndexBuffer() = default;

        virtual void Bind() = 0;

        virtual OxideError BufferData(const size_t size, const void *data) = 0;
        virtual OxideError AppendData(const size_t size, void* data) = 0;
        virtual OxideError Allocate(const size_t size) = 0;

        virtual const size_t& GetBufferSize() = 0;

        static Ref<IndexBuffer> Create();

    };

    struct BufferElement {
        size_t TypeSize;
        OxideType type;
        uint count;
    };

    class VertexBuffer { //TODO: Implement draw modes
    
    public:

        virtual ~VertexBuffer() = default;

        virtual void Bind() = 0;

        virtual void AssociateIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0;
        virtual const Ref<IndexBuffer>& GetAssociatedIndexBuffer() const = 0;

        virtual OxideError DrawElements(int count = -1) = 0; //If count is negative, it will draw the maximum number of indices.
        virtual OxideError DrawArrays(int count = -1) = 0; //If count is negative, it will draw the maximum number of vertices.

        virtual OxideError BufferData(const size_t size, void* data) = 0;   
        virtual OxideError AppendData(const size_t size, void* data) = 0;
        virtual OxideError Allocate(const size_t size) = 0;
        virtual size_t GetBufferSize() const = 0;

        virtual const std::vector<BufferElement>& GetBufferLayout() const;
        virtual void AddElementToBufferLayout(const BufferElement& element);
        virtual void AddElementToBufferLayout(const OxideType& type, const uint& count);
        virtual void AddElementToBufferLayout(const size_t& typeSize, const OxideType& type, const uint& count);
        virtual void AddElementsToBufferLayout(const std::vector<BufferElement>& elements);
        virtual void SetBufferLayout(const std::vector<BufferElement>& elements);
        virtual void ClearBufferLayout();

        static Ref<VertexBuffer> Create();

    protected:

        virtual void OnBufferLayoutChange() = 0;

        std::vector<BufferElement> m_BufferLayout;

    };
}