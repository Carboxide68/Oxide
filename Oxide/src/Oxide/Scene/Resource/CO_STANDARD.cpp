#include "Oxide/Scene/Resource/CO_STANDARD.h"

namespace Oxide {

    const std::string CO_STANDARD_RENDERER::Name = "CO_STANDARD_RENDERER";
    CO_STANDARD_RENDERER::CO_STANDARD_RENDERER() {
        m_VB = VertexBuffer::Create();
        m_IB = IndexBuffer::Create();

        m_VB->AssociateIndexBuffer(m_IB);

        //TODO: Make project wide path available
        m_Shader = Shader::Create("Oxide/src/Oxide/Scene/Resource/Storage/CO_SHADER.os");
        
        Uniform<int> *usingNormal = new Uniform<int>("usingNormal", 0);
        Uniform<int> *usingTexCoord = new Uniform<int>("usingTexCoord", 0);
        Uniform<int> *usingDiffuseTexture = new Uniform<int>("usingDiffuseTexture", 0);
        Uniform<int> *usingSpecularTexture = new Uniform<int>("usingSpecularTexture", 0);
        Uniform<glm::mat4> *assembledMatrix = new Uniform<glm::mat4>("assembledMatrix", glm::mat4(1));
        Uniform<glm::mat4> *modelMatrix = new Uniform<glm::mat4>("modelMatrix", glm::mat4(1));
        Uniform<glm::mat4> *normalMatrix = new Uniform<glm::mat4>("normalMatrix", glm::mat4(1));
        Uniform<Ref<Texture2D>> *diffuseTexture = new Uniform<Ref<Texture2D>>("diffuseTexture", nullptr);
        Uniform<Ref<Texture2D>> *specularTexture = new Uniform<Ref<Texture2D>>("specularTexture", nullptr);

        Uniform<glm::vec3> *diffuseColor = new Uniform<glm::vec3>("diffuseColor", glm::vec3(1));
        Uniform<float> *specularStrength = new Uniform<float>("specularStrength", 0.0f);

        m_Uniforms.push_back(usingNormal);
        m_Uniforms.push_back(usingTexCoord);
        m_Uniforms.push_back(usingDiffuseTexture);
        m_Uniforms.push_back(usingSpecularTexture);
        m_Uniforms.push_back(assembledMatrix);
        m_Uniforms.push_back(modelMatrix);
        m_Uniforms.push_back(normalMatrix);
        m_Uniforms.push_back(diffuseTexture);
        m_Uniforms.push_back(specularTexture);
        m_Uniforms.push_back(diffuseColor);
        m_Uniforms.push_back(specularStrength);

    }

    CO_STANDARD_RENDERER::~CO_STANDARD_RENDERER() {
        for (auto uniform : m_Uniforms) {
            delete uniform;
        }
    }

    Ref<Renderer> CO_STANDARD_RENDERER::Create() {
        return CreateRef<CO_STANDARD_RENDERER>();
    }

    void CO_STANDARD_RENDERER::Draw(const Renderable *object) {
        CO_STANDARD_RENDERABLE *RenderableObject = (CO_STANDARD_RENDERABLE*)(object);
        if (RenderableObject->GetRendererName() != Name) {
            CO_ERROR("Renderable object not suitble for rendering with %s renderer!", Name);
            return;
        }
        Load(RenderableObject->GetStandardRenderData());
        m_Shader->Bind();
        for (auto uniform : m_Uniforms) {
            uniform->SetUniform(&*m_Shader);
        }
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
            //Rearranging the data so that it is woven instead of seperated
            memcpy(&wovenData[arrayHead], &data.vertices[i], 3 * sizeof(float));
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