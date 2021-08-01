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

    /**
     * 
     * 
     * \struct
     * Struct containing information about a vertex in a Vertex Buffer
     * 
     */
    struct BufferElement {
        size_t TypeSize;
        OxideType type;
        uint count;
    };

    class VertexBuffer { //TODO: Implement draw modes
    
    public:

        virtual ~VertexBuffer() = default;

        virtual void Bind() = 0;

        virtual void AssociateIndexBuffer(const Ref<IndexBuffer> indexBuffer) = 0;
        virtual const Ref<IndexBuffer>& GetAssociatedIndexBuffer() const = 0;

        /**
         * @brief Draw using an index buffer
         * 
         * @param count Number of vertices to draw
         * @param offset Offset in bytes to first index
         * @param basevertex Offset to the first element in the vertex array, in count
         * @return OxideError 
         */
        virtual OxideError DrawElements(int count = -1, const size_t offset = 0, const size_t basevertex = 0) = 0; //If count is negative, it will draw the maximum number of indices.

        /**
         * @brief Draw vertices directly
         * 
         * @param count Number of vertices to draw
         * @param offset Offset to the first element, in count
         * @return OxideError 
         */
        virtual OxideError DrawArrays(int count = -1, size_t offset = 0) = 0; //If count is negative, it will draw the maximum number of vertices.

        virtual OxideError BufferData(const size_t size, void* data) = 0;   
        virtual OxideError SubData(const size_t begin, const size_t size, void* data) = 0;
        virtual OxideError AppendData(const size_t size, void* data) = 0;
        virtual OxideError Allocate(const size_t size) = 0;
        virtual size_t GetBufferSize() const = 0;

        virtual const std::vector<BufferElement>& GetBufferLayout() const;

        /**
         * Be careful using a custom position since it changes the behaviour of future AddElementsToBufferLayout
         */ 
        virtual void AddElementToBufferLayout(const BufferElement& element, int position = -1);
        virtual void AddElementToBufferLayout(const OxideType& type, const uint& count);
        virtual void AddElementToBufferLayout(const size_t& typeSize, const OxideType& type, const uint& count);
        virtual void AddElementsToBufferLayout(const std::vector<BufferElement>& elements);
        virtual void SetBufferLayout(const std::vector<BufferElement>& elements);
        virtual void ClearBufferLayout(); //TODO: Allow changing mode of buffer layout to AAAABBBBCCCC or ABCABCABCABC

        static Ref<VertexBuffer> Create();

    protected:

        virtual void OnBufferLayoutChange() = 0;

        std::vector<BufferElement> m_BufferLayout;

    };
}