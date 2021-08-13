#include "MeshRenderer.h"
#include "Oxide/Core/Log.h"
#include "Oxide/Renderer/Renderer.h"

#define DRAWMODE_DRAW_ARRAYS (1 << 0)
#define DRAWMODE_DRAW_ELEMENTS (1 << 1)

#define DATAPROPS_USING_NORMALS (1 << 0)
#define DATAPROPS_USING_TEX_COORDS (1 << 1)

namespace Oxide {

    MeshRenderer::MeshRenderer(Scene* parentScene) : ORenderer(parentScene) {

        m_VertexBuffer = VertexBuffer::Create();
        m_IndexBuffer = IndexBuffer::Create();
        m_VertexBuffer->AssociateIndexBuffer(m_IndexBuffer);
        m_IndexBuffer->Alloc(1000000);
        m_VertexBuffer->Alloc(10000000);

        m_Shader = Shader::Create("Oxide/src/Oxide/Resource/MeshShader.os");
        m_IdLookup.reserve(1000);

    }

    void MeshRenderer::Draw(uint &id, const Mesh& mesh, bool update) {
        ZoneScoped
        if (m_IdLookup.find(id) == m_IdLookup.end()) {
            id = 0;
        }

        if (!id || update) {
            Load(id, mesh);
        }
        DrawObject* object = &m_IdLookup[id];
        object->DrawMode = (mesh.UsingIndices) ? DRAWMODE_DRAW_ELEMENTS : DRAWMODE_DRAW_ARRAYS;
        m_Draws.push_back({id, object->ModelMatrix});
    }

    void MeshRenderer::DrawInstance(uint& id, const glm::mat4& transform) {
        if (id == 0) return;
        if (m_IdLookup.find(id) == m_IdLookup.end()) return;

        m_Draws.push_back({id, transform});
    }

    void MeshRenderer::Load(uint &id, const Mesh& mesh) {
        if (!id) {
            id = m_IdTally++;
            m_IdLookup[id] = {};
        }
        DrawObject* object = &m_IdLookup[id];
        object->Material = mesh.Material;

        object->DataProps |= (mesh.UsingNormals) ? DATAPROPS_USING_NORMALS : 0;
        object->DataProps |= (mesh.UsingTexCoords) ? DATAPROPS_USING_TEX_COORDS : 0;

        uint alignamounts[] = {sizeof(float) * 3, sizeof(float) * 6, sizeof(float) * 5, sizeof(float) * 8};
        Align(alignamounts[object->DataProps]);

        uint size = 0;
        //Get float data from the mesh as well as the bytesize of that data
        //Document the bufferlocation of this material and increase the bufferhead
        //Add the data to the buffer
        {
        std::vector<float> data = mesh.GetVertexData(size);
        object->VertexBufferLocation = {m_VertexBufferHead, m_VertexBufferHead + size};
        m_VertexBufferHead += size;
        m_VertexBuffer->AppendData(size, data.data());
        }

        size = 0;
        //Get index data from the mesh as well as the bytesize of that data
        //Document the position in the gpu buffer that this is located
        //Add the data to the index buffer
        {
        std::vector<uint> data = mesh.GetIndexData(size);
        object->IndexBufferLocation = {m_IndexBufferHead, m_IndexBufferHead + size};
        m_IndexBufferHead += size;
        m_IndexBuffer->AppendData(size, data.data());
        }
        //Drawcount should be number of vertices, which in this case is the number of faces multiplied by 3
        object->DrawCount = mesh.Faces.size() * 3;
        object->ModelMatrix = mesh.ModelMatrix;

    }

    std::vector<std::pair<int, std::function<void()>>> MeshRenderer::Queue() {

        //If the amount of unpopulated space in the vertex buffer reaches a threshhold,
        //we will reorganize the buffer to remove unused space
        if (m_VertexBufferHead != 0) {
            size_t populated_buffer_size = m_Padding;
            for (auto i = m_IdLookup.begin(); i != m_IdLookup.end(); i++) {
                populated_buffer_size += i->second.VertexBufferLocation.second - i->second.VertexBufferLocation.first;
            }
            if (((float)populated_buffer_size/(float)m_VertexBufferHead) < m_MinPercentPopulated) {
                CleanBuffers();
            }
        }
        return {{0, [&](void) ->  void {
            m_VertexBuffer->bufferLayout = {{sizeof(float), OxideType::Float, 3}, {sizeof(float), OxideType::Float, 3}, {sizeof(float), OxideType::Float, 2}};
            m_VertexBuffer->UpdateLayout();
            Ref<Camera> camera = m_ParentScene->camera;
            ZoneScopedN("Draw meshes");
            m_VertexBuffer->Bind();
            for (InstanceData draw : m_Draws) {
                ZoneScopedN("Draw Mesh")
                DrawObject& object = m_IdLookup[draw.id];
                m_Shader->Bind();
                object.Material.SetUniforms(m_Shader);
                m_Shader->SetUniform("uAssembledMatrix", camera->GetPerspectiveMatrix() * camera->GetViewMatrix() * draw.transform);
                m_Shader->SetUniform("uModelMatrix", draw.transform);
                m_Shader->SetUniform("uInverseModel", glm::inverse(draw.transform));

                size_t vertexSize = 1;
                
                m_VertexBuffer->EnableAttrib(0);
                switch (object.DataProps) {
                    case 0:
                        m_VertexBuffer->DisableAttrib(1);
                        m_VertexBuffer->DisableAttrib(2);
                        vertexSize = 3;
                        break;
                    case 1: //Only using normals
                        m_VertexBuffer->EnableAttrib(1);
                        m_VertexBuffer->DisableAttrib(2);
                        vertexSize = 6;
                        break;
                    case 2: //Only using TexCoords
                        m_VertexBuffer->DisableAttrib(1);
                        m_VertexBuffer->EnableAttrib(2);
                        vertexSize = 5;
                        break;
                    case 3: //Using both
                        m_VertexBuffer->EnableAttrib(1);
                        m_VertexBuffer->EnableAttrib(2);
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
        size_t oldsize = m_VertexBufferHead;

        Ref<VertexBuffer> oldVertexBuffer = m_VertexBuffer;
        Ref<IndexBuffer> oldIndexBuffer = m_IndexBuffer;
        m_VertexBuffer = VertexBuffer::Create();
        m_VertexBuffer->bufferLayout = oldVertexBuffer->bufferLayout; 
        m_VertexBuffer->Alloc(m_VertexBufferHead);

        if (oldIndexBuffer) {
            m_IndexBuffer = IndexBuffer::Create();
            m_IndexBuffer->Alloc(m_IndexBufferHead);
            m_IndexBufferHead = 0;
            m_VertexBuffer->AssociateIndexBuffer(m_IndexBuffer);
        }

        m_Padding = 0;
        m_VertexBufferHead = 0;
        for (auto& idpair : m_IdLookup) {
            auto &object = idpair.second;
            m_VertexBuffer->bufferPosition = m_VertexBufferHead;
            uint alignamounts[] = {sizeof(float) * 3, sizeof(float) * 6, sizeof(float) * 5, sizeof(float) * 8};
            Align(alignamounts[object.DataProps]);

            uint size = object.VertexBufferLocation.second - object.VertexBufferLocation.first;
            oldVertexBuffer->CopyData(m_VertexBuffer, object.VertexBufferLocation.first, m_VertexBufferHead, size);
            object.VertexBufferLocation.first = m_VertexBufferHead;
            object.VertexBufferLocation.second = m_VertexBufferHead + size;
            m_VertexBufferHead += size;

            if (oldIndexBuffer) {
                m_IndexBuffer->bufferPosition = m_IndexBufferHead;
                size = object.IndexBufferLocation.second - object.IndexBufferLocation.first;
                oldIndexBuffer->CopyData(m_IndexBuffer, object.IndexBufferLocation.first, m_IndexBufferHead, size);
                object.IndexBufferLocation.first = m_IndexBufferHead;
                object.IndexBufferLocation.second = m_IndexBufferHead + size;
                m_IndexBufferHead += size;
            }
        }
        m_VertexBuffer->bufferPosition = m_VertexBufferHead;
        m_IndexBuffer->bufferPosition = m_IndexBufferHead;
        m_VertexBuffer->UpdateLayout();
        CO_CORE_TRACE("[MeshRenderer::CleanBuffers] Decreased size by %luB, current size: %luB", oldsize - m_VertexBufferHead, m_VertexBufferHead);
    }

    void MeshRenderer::Align(size_t multiple) {
        uint alignamount = multiple - (m_VertexBufferHead%multiple);
        if (alignamount == multiple) return;
        std::vector<unsigned char> aligndata(alignamount, 0);
        m_VertexBuffer->AppendData(alignamount, aligndata.data());
        m_VertexBufferHead += alignamount;
        m_Padding += alignamount;
    }
}