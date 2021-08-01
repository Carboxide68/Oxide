#include "MeshRenderer.h"
#include "Oxide/Core/Log.h"

#define DRAWMODE_DRAW_ARRAYS (1 << 0)
#define DRAWMODE_DRAW_ELEMENTS (1 << 1)

#define DATAPROPS_USING_NORMALS (1 << 0)
#define DATAPROPS_USING_TEX_COORDS (1 << 1)

namespace Oxide {

    MeshRenderer::MeshRenderer(Scene* parentScene) : ORenderer(parentScene) {

        m_VertexBuffer = VertexBuffer::Create();
        m_IndexBuffer = IndexBuffer::Create();
        m_VertexBuffer->AssociateIndexBuffer(m_IndexBuffer);
        m_IndexBuffer->Allocate(1000000);
        m_VertexBuffer->Allocate(10000000);

        m_Shader = Shader::Create("Oxide/src/Oxide/Resource/MeshShader.os");
        m_IdLookup.reserve(1000);

    }

    void MeshRenderer::Draw(uint &id, const Mesh& mesh, bool update) {
        ZoneScoped
        DrawObject* object;
        if (m_IdLookup.find(id) == m_IdLookup.end()) {
            id = 0;
        }

        if (!id) {
            ZoneScopedN("Draw NewObject")
            id = m_IdTally++; //id becomes previous value of m_IdTally, whilst m_IdTally is incremented by 1
            m_IdLookup[id] = {};
            object = &m_IdLookup[id];
            object->ModelMatrix = mesh.ModelMatrix;
            object->Material = mesh.Material;
            uint size = 0;

            //Get float data from the mesh as well as the bytesize of that data
            //Document the bufferlocation of this material and increase the bufferhead
            //Add the data to the buffer
            {
            std::vector<float> data = mesh.GetVertexData(size);
            object->VertexBufferLocation = {m_VertexbufferHead, m_VertexbufferHead + size};
            m_VertexbufferHead += size;
            m_VertexBuffer->AppendData(size, data.data());
            }

            size = 0;
            //Get index data from the mesh as well as the bytesize of that data
            //Document the position in the gpu buffer that this is located
            //Add the data to the index buffer
            {
            std::vector<uint> data = mesh.GetIndexData(size);
            object->IndexBufferLocation = {m_IndexbufferHead, m_IndexbufferHead + size};
            m_IndexbufferHead += size;
            m_IndexBuffer->AppendData(size, data.data());
            }
            //Drawcount should be number of vertices, which in this case is the number of faces multiplied by 3
            object->DrawCount = mesh.Faces.size() * 3;

            object->DataProps |= (mesh.UsingNormals) ? DATAPROPS_USING_NORMALS : 0;
            object->DataProps |= (mesh.UsingTexCoords) ? DATAPROPS_USING_TEX_COORDS : 0;

        } else if (update) {
            ZoneScopedN("Update Object")
            object = &m_IdLookup[id];
            object->Material = mesh.Material;
            uint size = 0;
            {
            std::vector<float> data = mesh.GetVertexData(size);
            object->VertexBufferLocation = {m_VertexbufferHead, m_VertexbufferHead + size};
            m_VertexbufferHead += size;
            m_VertexBuffer->AppendData(size, data.data());
            }

            size = 0;
            //Get index data from the mesh as well as the bytesize of that data
            //Document the position in the gpu buffer that this is located
            //Add the data to the index buffer
            {
            std::vector<uint> data = mesh.GetIndexData(size);
            object->IndexBufferLocation = {m_IndexbufferHead, m_IndexbufferHead + size};
            m_IndexbufferHead += size;
            m_IndexBuffer->AppendData(size, data.data());
            }
            object->DrawCount = mesh.Faces.size() * 3;

            object->DataProps |= (mesh.UsingNormals) ? DATAPROPS_USING_NORMALS : 0;
            object->DataProps |= (mesh.UsingTexCoords) ? DATAPROPS_USING_TEX_COORDS : 0;

        } else {
            object = &m_IdLookup[id];
        }

        //Make sure that the buffer is aligned to a multiple of 3, 5, 6 and 8 floats, where the smallest one is 120
        uint alignment = (m_VertexbufferHead % (sizeof(float) * 120));
        if (alignment) {
            alignment = sizeof(float) * 120 - alignment;
            std::vector<char> alignmentData(alignment, 0);
            m_VertexBuffer->AppendData(alignment, alignmentData.data());
            m_VertexbufferHead += alignment;
        }

        object->DrawMode = (mesh.UsingIndices) ? DRAWMODE_DRAW_ELEMENTS : DRAWMODE_DRAW_ARRAYS;
        object->ModelMatrix = mesh.ModelMatrix;
        m_Draws.push_back(id);
    }

    std::vector<std::pair<int, std::function<void()>>> MeshRenderer::Queue() {

        //If the amount of unpopulated space in the vertex buffer reaches a threshhold, \
        //we will reorganize the buffer to remove unused space
        if (m_VertexbufferHead != 0) {
            size_t populated_buffer_size = 0;
            for (auto i = m_IdLookup.begin(); i < m_IdLookup.end(); i++) {
                populated_buffer_size += i->second - i->first;
            }
            if (populated_buffer_size/m_VertexbufferHead < m_MinPercentPopulated) {
                CleanBuffers();
            }
        }
        return {{0, [&](void) ->  void {
            Ref<Camera> camera = m_ParentScene->camera;
            ZoneScopedN("Draw meshes");
            m_VertexBuffer->Bind();
            for (uint draw : m_Draws) {
                ZoneScopedN("Draw Mesh")
                DrawObject& object = m_IdLookup[draw];
                m_Shader->Bind();
                object.Material.SetUniforms(m_Shader);
                m_Shader->SetUniform("uAssembledMatrix", camera->GetPerspectiveMatrix() * camera->GetViewMatrix() * object.ModelMatrix);
                m_Shader->SetUniform("uModelMatrix", object.ModelMatrix);
                m_Shader->SetUniform("uInverseModel", glm::inverse(object.ModelMatrix));

                size_t vertexSize = 1;
                
                switch (object.DataProps) {
                    case 0:
                        m_VertexBuffer->SetBufferLayout({{sizeof(float), OxideType::Float, 3}, {sizeof(float), OxideType::None, 0}, {sizeof(float), OxideType::None, 0}});
                        vertexSize = 3;
                        break;
                    case 1: //Only using normals
                        m_VertexBuffer->SetBufferLayout({{sizeof(float), OxideType::Float, 3}, {sizeof(float), OxideType::Float, 3}, {sizeof(float), OxideType::None, 0}});
                        vertexSize = 6;
                        break;
                    case 2: //Only using TexCoords
                        m_VertexBuffer->SetBufferLayout({{sizeof(float), OxideType::Float, 3}, {sizeof(float), OxideType::None, 0}, {sizeof(float), OxideType::Float, 2}});
                        vertexSize = 5;
                        break;
                    case 3: //Using both
                        m_VertexBuffer->SetBufferLayout({{sizeof(float), OxideType::Float, 3}, {sizeof(float), OxideType::Float, 3}, {sizeof(float), OxideType::Float, 2}});
                        vertexSize = 8;
                        break;
                    default:
                        CO_CORE_WARN("DataProps in MeshRenderer weren't the expected value!");
                        continue;
                }

                switch (object.DrawMode) {
                    case (DRAWMODE_DRAW_ARRAYS):
                        m_VertexBuffer->DrawArrays(object.DrawCount, object.VertexBufferLocation.first);
                        break;

                    case (DRAWMODE_DRAW_ELEMENTS):
                        m_VertexBuffer->DrawElements(object.DrawCount, object.IndexBufferLocation.first, object.VertexBufferLocation.first/(vertexSize * sizeof(float)));                        
                        break;
                    default:
                        continue;
                }
            }
            m_Draws.clear();
        }}};
    }

    void MeshRenderer::CleanBuffers() {
        //TODO: Implement this!
    }
}