#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/VertexArray.h"

namespace Oxide {

    /**
     * @brief Type to describe characteristics of an attribute in a vertex array
     * 
     */
    struct BufferElement {
        size_t TypeSize;
        OxideType type;
        uint count;
        bool enabled = true;
    };

    class VertexBuffer { //TODO: Give parent class buffer that exposes buffer general functionality. Maybe move Buffer, Alloc etc there

    public:

        ~VertexBuffer();
        void Bind() override;

        void AssociateIndexBuffer(const Ref<IndexBuffer> indexBuffer);
        Ref<IndexBuffer> GetAssociatedIndexBuffer() const;


        /**
         * @brief Pushes BufferElements as the buffer layout 
         * 
         */
        void UpdateLayout();

        /**
         * @brief Quick way to enable an attribute without having to update the layout. Does not update the associated BufferElement.
         * 
         */
        void EnableAttrib(uint attrib);

        /**
         * @brief Quick way to disable an attribute without having to update the layout. Does not update the associated BufferElement.
         * 
         */
        void DisableAttrib(uint attrib);


        /**
         * @brief Draws elements in current buffer. Requires an associated index buffer
         * 
         * @param count The amount of vertices that should be drawn
         * @param offset Offset to the first value in the index buffer
         * @param basevertex Offset to the first vertex in the vertex buffer
         * @return OxideError 
         */
        OxideError DrawElements(const size_t count, const size_t offset = 0, const size_t basevertex = 0); //If count is negative, it will draw the maximum number of indices.

        /**
         * @brief Draw arrays from current buffer
         * 
         * @param count The amount of vertices to be drawn
         * @param offset Offset to the first vertex in the vertex buffer 
         * @return OxideError 
         */
        OxideError DrawArrays(const size_t count, const size_t offset = 0); //If count is negative, it will draw the maximum number of vertices.

        /**
         * @brief Creates a new buffer with data
         * 
         * @param size Bytesize of data, also the size of the buffer to be allocated
         * @param data Data
         * @return OxideError 
         */
        OxideError BufferData(const size_t size, void* data);

        /**
         * @brief Buffer data into a portion of the bound buffer.
         * 
         * @param begin Offset to where to start buffering data
         * @param size Size of the data to be buffered
         * @param data The data to be buffered
         * @return OxideError 
         */
        OxideError SubData(const size_t begin, const size_t size, void* data);

        /**
         * @brief Buffer data to the end of the buffer. Creates a new buffer if size + bufferhead > buffersize
         * 
         * @param size Size of data to be appended
         * @param data Data to be buffered
         * @return OxideError 
         */
        OxideError AppendData(const size_t size, void* data);

        /**
         * @brief Allocate data in a buffer.
         * 
         * @param size Amount to be allocated
         * @return OxideError 
         */
        OxideError Alloc(const size_t size);

        inline uint32_t GetHandle() {return m_RendererID;}

        size_t GetBufferSize() const;

        static Ref<VertexBuffer> Create();

        std::vector<BufferElement> BufferLayout;

    private:

        VertexBuffer();

        Ref<VertexArray> m_VAO;
        uint32_t m_RendererID;
        Ref<IndexBuffer> m_IndexBuffer;

        size_t m_BufferSize;
        size_t m_BufferPosition;
        size_t m_BufferStride;
        
    };

    class IndexBuffer {

    public:

        ~IndexBuffer();
        void Bind();

        OxideError BufferData(const size_t size, const void *data);
        OxideError AppendData(const size_t size, void* data);
        OxideError Alloc(const size_t size);

        const size_t& GetBufferSize();

        inline uint32_t GetHandle() {return m_RendererID};

        static Ref<IndexBuffer> Create();

    private:
        IndexBuffer();

        uint32_t m_RendererID;
        size_t m_BufferSize;
        size_t m_BufferPosition;

    };

}