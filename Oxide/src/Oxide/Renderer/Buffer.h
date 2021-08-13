#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/VertexArray.h"
#include "Oxide/Renderer/OpenGLCommon.h"

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

    class Buffer {

    public:

        ~Buffer();
        
        void Bind();

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

        /**
         * @brief Copies data from one GPU buffer to another
         * 
         * @param other Write target
         * @param buffer0_offset Offset in first buffer
         * @param buffer1_offset Offset in second buffer
         * @param size Amount of bytes to copy
         * @return OxideError 
         */
        OxideError CopyData(Ref<Buffer> other, size_t buffer0_offset, size_t buffer1_offset, size_t size);

        inline uint32_t GetHandle() const {return m_RendererID;}
        inline GLenum GetBufferUse() const {return m_BufferUse;}
        inline GLenum GetBufferType() const {return m_BufferType;}

        size_t GetBufferSize() const;

        static Ref<Buffer> Create(GLenum bufferType, GLenum bufferUse);

        size_t bufferPosition = 0;
    
    protected:

        Buffer(GLenum bufferType, GLenum bufferUse);

        GLenum m_BufferType;
        GLenum m_BufferUse;
        size_t m_BufferSize = 0;
        uint32_t m_RendererID;
    };

    class IndexBuffer;

    class VertexBuffer : public Buffer {

    public:

        ~VertexBuffer();
        void Bind();

        void AssociateIndexBuffer(const Ref<IndexBuffer> indexBuffer);
        Ref<IndexBuffer> GetAssociatedIndexBuffer() const;

        /**
         * @brief Draws elements in current buffer. Requires an associated index buffer
         * 
         * @param count The amount of vertices that should be drawn
         * @param offset Offset to the first value in the index buffer
         * @param basevertex Offset to the first vertex in the vertex buffer
         * @return OxideError 
         */
        OxideError DrawElements(const size_t count, const size_t offset = 0, const size_t basevertex = 0);

        /**
         * @brief Draw arrays from current buffer
         * 
         * @param count The amount of vertices to be drawn
         * @param offset Offset to the first vertex in the vertex buffer 
         * @return OxideError 
         */
        OxideError DrawArrays(const size_t count, const size_t offset = 0);

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

        static Ref<VertexBuffer> Create();
        static Ref<VertexBuffer> Create(uint32_t handle, size_t buffersize);

        std::vector<BufferElement> bufferLayout;

    private:

        VertexBuffer();
        VertexBuffer(uint32_t handle, size_t buffersize);

        Ref<VertexArray> m_VAO;
        Ref<IndexBuffer> m_IndexBuffer;
        
    };

    class IndexBuffer : public Buffer {

    public:

        ~IndexBuffer();

        static Ref<IndexBuffer> Create();

    private:
        IndexBuffer();

    };

}