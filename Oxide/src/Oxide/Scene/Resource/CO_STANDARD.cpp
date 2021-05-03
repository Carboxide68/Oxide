#include "Oxide/Scene/Resource/CO_STANDARD.h"

namespace Oxide {

    CO_STANDARD_RENDERER::CO_STANDARD_RENDERER() {
        m_VB = VertexBuffer::Create();
        m_IB = IndexBuffer::Create();

        m_VB->AssociateIndexBuffer(m_IB);

        m_Shader = Shader::Create("Oxide/src/Oxide/Scene/Resource/Storage/CO_SHADER.os");

    }

    Ref<Renderer> CO_STANDARD_RENDERER::Create() {
        return CreateRef<CO_STANDARD_RENDERER>();
    }

    void CO_STANDARD_RENDERER::Draw(Renderable *object) {
        CO_STANDARD_RENDERABLE *RenderableObject = (CO_STANDARD_RENDERABLE*)(object);
        if (RenderableObject->GetRendererName() != m_Name) {
            CO_ERROR("Renderable object not suitble for rendering with %s renderer!", m_Name);
            return;
        }
        Load(RenderableObject->GetStandardRenderData());
    }

    void CO_STANDARD_RENDERER::Load(const StandardRenderData data) {

        bool usingIndices, usingNormals, usingTexCoords = false;
        //Check what we're using
        usingIndices = data.indices->size();
        usingNormals = data.normals->size();
        usingTexCoords = data.texCoords->size();

        m_VB->ClearBufferLayout();
        m_VB->AddElementToBufferLayout(Oxide::Float, 3);
        if (usingNormals) 
            m_VB->AddElementToBufferLayout({sizeof(float), Oxide::Float, 3}, 1);
        if (usingTexCoords)
            m_VB->AddElementToBufferLayout({sizeof(float), Oxide::Float, 2}, 2);
        if (usingIndices)
            m_IB->BufferData(data.indices->size() * sizeof(unsigned short), data.indices->data());
        
        float wovenData[data.vertices->size() * 3 + data.normals->size() * 3 + data.texCoords->size() * 2];
        uint arrayHead = 0;
        for (uint i = 0; i < data.vertices->size(); i++) {
            memcpy(&wovenData[arrayHead], &data.vertices[i], 3 * sizeof(float)); //Rearranging the data so that it is woven instead of seperated
            arrayHead += 3;
            if (usingNormals) {
                memcpy(&wovenData[arrayHead], &data.normals[i], 3 * sizeof(float));
                arrayHead += 3;
            }
            if (usingTexCoords) {
                memcpy(&wovenData[arrayHead], &data.texCoords[i], 2 * sizeof(float));
                arrayHead += 2;
            }
        }
        m_VB->BufferData(sizeof(wovenData), (void*) wovenData);
    }
}