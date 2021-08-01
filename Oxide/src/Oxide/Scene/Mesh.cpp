#include "Oxide/Scene/Mesh.h"
#include "Oxide/Renderer/Shader.h"

namespace Oxide {

    Mesh::Mesh() {
        ModelMatrix = glm::mat4(1);
    }

    void Mesh::SetScale(const float& size) {
        ModelMatrix[0][0] *= size;
        ModelMatrix[1][0] *= size;
        ModelMatrix[2][0] *= size;
        ModelMatrix[0][1] *= size;
        ModelMatrix[1][1] *= size;
        ModelMatrix[2][1] *= size;
        ModelMatrix[0][2] *= size;
        ModelMatrix[1][2] *= size;
        ModelMatrix[2][2] *= size;
    }

    std::vector<float> Mesh::GetVertexData(uint &size, bool weave) const {
        ZoneScopedN("Mesh GetVertex");
        size_t floatsPerVertex = 3 + (UsingNormals ? 3 : 0) + (UsingTexCoords ? 2 : 0);
        size = floatsPerVertex * sizeof(float) * Vertices.size();
        std::vector<float> data(floatsPerVertex * Vertices.size());
        if (weave) {
            size_t bufpos = 0;
            for (size_t i = 0; i < Vertices.size();i++) {
                data[bufpos] = Vertices[i][0];
                data[bufpos+1] = Vertices[i][1];
                data[bufpos+2] = Vertices[i][2];
                bufpos += 3;
                if (UsingNormals) {
                    data[bufpos] = Normals[i][0];
                    data[bufpos+1] = Normals[i][1];
                    data[bufpos+2] = Normals[i][2];
                    bufpos += 3;
                }
                if (UsingTexCoords) {
                    data[bufpos] = TexCoords[i][0];
                    data[bufpos+1] = TexCoords[i][1];
                    bufpos += 2;
                }
            }
        } else {
            size_t bufpos = 0;
            for (size_t i = 0; i < Vertices.size(); i++) {
                data[bufpos] = Vertices[i][0];
                data[bufpos+1] = Vertices[i][1];
                data[bufpos+2] = Vertices[i][2];
                bufpos += 3;
            }
            if (UsingNormals) {
                for (size_t i = 0; i < Vertices.size(); i++) {
                    data[bufpos] = Vertices[i][0];
                    data[bufpos+1] = Vertices[i][1];
                    data[bufpos+2] = Vertices[i][2];
                    bufpos += 3;
                }
            }
            if (UsingTexCoords) {
                for (size_t i = 0; i < Vertices.size(); i++) {
                    data[bufpos] = Vertices[i][0];
                    data[bufpos+1] = Vertices[i][1];
                    bufpos += 2;
                }
            }
        }
        return data;
    }

    std::vector<uint> Mesh::GetIndexData(uint &size) const {
        size = Faces.size() * 3 * sizeof(uint);
        std::vector<uint> data(Faces.size() * 3);
        for (size_t i = 0; i < Faces.size(); i++) {
            data[i*3] = Faces[i][0];
            data[i*3+1] = Faces[i][1];
            data[i*3+2] = Faces[i][2];
        }
        return data;
    }

    void MeshMaterial::SetUniforms(Ref<Shader> shader) {
        shader->SetUniform("uMatProps", (uint)MatProps);
        shader->SetUniform("uAmbientTexture", (int)0);
        shader->SetUniform("uDiffuseTexture", (int)1);
        shader->SetUniform("uSpecularTexture", (int)2);
        shader->SetUniform("uShininessTexture", (int)3);
        shader->SetUniform("uDisplacementTexture", (int)4);

        if (Textures[0]) Textures[0]->Bind(0);
        if (Textures[1]) Textures[1]->Bind(1);
        if (Textures[2]) Textures[2]->Bind(2);
        if (Textures[3]) Textures[3]->Bind(3);
        if (Textures[4]) Textures[4]->Bind(4);

        shader->SetUniform("uShininess", Shininess);
        shader->SetUniform("uAmbientColor", AmbientColor);
        shader->SetUniform("uDiffuseColor", DiffuseColor);
        shader->SetUniform("uSpecularColor", SpecularColor);
    }
}