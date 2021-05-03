#include "Oxide/Scene/Resource/Renderer.h"

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

    void CO_STANDARD_RENDERER::Draw() {

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
            wovenData[arrayHead] = data.vertices[i][0];
            wovenData[arrayHead + 1] = data.vertices[i][1];
            wovenData[arrayHead + 2] = data.vertices[i][2];
            arrayHead += 3;
            if (usingNormals) {
                wovenData[arrayHead] = data.normals[i][0];
                wovenData[arrayHead + 1] = data.normals[i][1];
                wovenData[arrayHead + 2] = data.normals[i][2];
                arrayHead += 3;
            }
            if (usingTexCoords) {
                wovenData[arrayHead] = data.texCoords[i][0];
                wovenData[arrayHead + 1] = data.texCoords[i][1];
                arrayHead += 2;
            }
        }
        m_VB->BufferData(sizeof(wovenData), (void*) wovenData);

    }

}