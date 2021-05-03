
#include "Platform/OpenGL/OpenGLCommon.h"

#include "Oxide/Renderer/Buffer.h"
#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Oxide/Renderer/CRenderer.h"


namespace Oxide {

    Ref<VertexBuffer> VertexBuffer::Create() {

        switch (CRenderer::GetAPI()) {
            case CRenderer::API::None: CO_CORE_ASSERT(false, "Renderer::API::None is currently not supported!"); return nullptr;
            case CRenderer::API::OpenGL: return CreateRef<OpenGLVertexBuffer>();
            default: break;
        }

        CO_CORE_ASSERT(false, "This RendererAPI doesn't exist!");
        return nullptr;
    }

    const std::vector<BufferElement>& VertexBuffer::GetBufferLayout() const {

        return m_BufferLayout;

    }

    void VertexBuffer::AddElementToBufferLayout(const BufferElement& element, int position) {
        if (position != -1) {
            if (position > m_BufferLayout.size()) {
                m_BufferLayout.resize(position + 1, {OxideType::None, });
            }
            m_BufferLayout[position] = element;
        } else {
            m_BufferLayout.push_back(element);
        }
        OnBufferLayoutChange();
    }

    void VertexBuffer::AddElementsToBufferLayout(const std::vector<BufferElement>& elements) {
        m_BufferLayout.insert(m_BufferLayout.end(), elements.begin(), elements.end());
        OnBufferLayoutChange();
    }

    void VertexBuffer::AddElementToBufferLayout(const OxideType& type, const uint& count) {
        m_BufferLayout.push_back({OpenGLGetOxideTypeSize(type), type, count});
        OnBufferLayoutChange();
    }

    void VertexBuffer::AddElementToBufferLayout(const size_t& typeSize, const OxideType& type, const uint& count) {
        m_BufferLayout.push_back({typeSize, type, count});
        OnBufferLayoutChange();
    }

    void VertexBuffer::SetBufferLayout(const std::vector<BufferElement>& elements) {
        m_BufferLayout = elements;
        OnBufferLayoutChange();
    }


    void VertexBuffer::ClearBufferLayout() {
        m_BufferLayout.clear();
        OnBufferLayoutChange();
    }

    Ref<IndexBuffer> IndexBuffer::Create() {

        switch (CRenderer::GetAPI()) {
            case CRenderer::API::None: CO_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case CRenderer::API::OpenGL: return CreateRef<OpenGLIndexBuffer>();
            default: break;
        }

        CO_CORE_ASSERT(false, "This RendererAPI doesn't exist!");
        return nullptr;
    }

    

}