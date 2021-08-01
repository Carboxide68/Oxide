
#include "Oxide/Core/Log.h"
#include "Oxide/Scene/Model.h"
#include "Platform/OpenGL/OpenGLCommon.h"
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>

namespace Oxide {

    Assimp::Importer Model::importer = Assimp::Importer();

    OxideError Model::LoadModel(const std::string& filePath) {

        ZoneScoped
        Meshes.clear();

        const aiScene* scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Quality | aiProcess_OptimizeMeshes);

        if (!scene) {
            CO_CORE_WARN("Failed to load model at path: %s!", filePath.c_str());
            return OxideError::Error;
        }

        uint lastslash = 0;
        for (uint i = 0; i < filePath.size(); i++) {
            if (filePath[i] == '/') lastslash = i;
        }
        lastslash += 1;
        std::string directory(filePath.begin(), filePath.begin() + lastslash);

        for (uint i = 0; i < scene->mNumMeshes; i++) {
            #ifdef TRACY_ENABLE
            if ((i - 50) > 0 && (i % 50) == 0) {
                TracyGpuCollect
            }
            #endif
            ZoneScopedN("MeshLoad")
            Meshes.push_back(Mesh());

            Mesh& curMesh = Meshes.back();
            const aiMesh* curaiMesh = scene->mMeshes[i];
            curMesh.Vertices.resize(curaiMesh->mNumVertices);

            //TODO: Make this more capable
            for (size_t j = 0; j < curaiMesh->mNumVertices; j++) {
                curMesh.Vertices[j][0] = curaiMesh->mVertices[j][0];
                curMesh.Vertices[j][1] = curaiMesh->mVertices[j][1];
                curMesh.Vertices[j][2] = curaiMesh->mVertices[j][2];
            }

            curMesh.UsingNormals = curaiMesh->HasNormals();
            if (curMesh.UsingNormals) {
                curMesh.Normals.resize(curaiMesh->mNumVertices);
                for (size_t j = 0; j < curaiMesh->mNumVertices; j++) {
                    curMesh.Normals[j][0] = curaiMesh->mNormals[j][0];
                    curMesh.Normals[j][1] = curaiMesh->mNormals[j][1];
                    curMesh.Normals[j][2] = curaiMesh->mNormals[j][2];
                }
            }

            curMesh.UsingTexCoords = curaiMesh->HasTextureCoords(0);
            if (curMesh.UsingTexCoords) {
                curMesh.TexCoords.resize(curaiMesh->mNumVertices);
                for (size_t j = 0; j < curaiMesh->mNumVertices; j++) {
                    curMesh.TexCoords[j][0] = curaiMesh->mTextureCoords[0][j][0];
                    curMesh.TexCoords[j][1] = curaiMesh->mTextureCoords[0][j][1];
                }
            }

            curMesh.UsingIndices = curaiMesh->HasFaces();
            if (curMesh.UsingIndices) {
                curMesh.Faces.reserve(curaiMesh->mNumFaces);
                for (uint j = 0; j < curaiMesh->mNumFaces; j++) {
                    curMesh.Faces.push_back({curaiMesh->mFaces[j].mIndices[0], curaiMesh->mFaces[j].mIndices[1], curaiMesh->mFaces[j].mIndices[2]});
                }
            }

            if (scene->HasMaterials()) {
                aiMaterial* mat = scene->mMaterials[curaiMesh->mMaterialIndex];
                aiColor3D color (-1.f, 1.f, 1.f);
                aiString aitex;
                
                MeshMaterial& material = curMesh.Material;
                mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
                if (color[0] != -1.f) {
                    material.MatProps &= ~(MATPROPS_USING_AMBIENT_TEXTURE);
                    material.AmbientColor = {color[0], color[1], color[2]};
                }

                mat->Get(AI_MATKEY_TEXTURE_AMBIENT(0), aitex);
                if (aitex.length) {
                    material.Textures[0] = TextureLib.Get(directory + std::string(aitex.C_Str()));
                    if (!material.Textures[0]) {
                        material.Textures[0] = Texture2D::Create(directory + std::string(aitex.C_Str()));
                        TextureLib.Add(material.Textures[0]);
                    }
                    material.MatProps |= MATPROPS_USING_AMBIENT_TEXTURE;
                }


                color = {-1.f, 1.f, 1.f};
                mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
                if (color[0] != -1.f) {
                    material.MatProps &= ~(MATPROPS_USING_DIFFUSE_TEXTURE);
                    material.DiffuseColor = {color[0], color[1], color[2]};
                }

                mat->Get(AI_MATKEY_TEXTURE_DIFFUSE(0), aitex);
                if (aitex.length) {
                    material.Textures[1] = TextureLib.Get(directory + std::string(aitex.C_Str()));
                    if (!material.Textures[1]) {
                        material.Textures[1] = Texture2D::Create(directory + std::string(aitex.C_Str()));
                        TextureLib.Add(material.Textures[1]);
                    }
                    material.MatProps |= MATPROPS_USING_DIFFUSE_TEXTURE;
                }
                aitex.Clear();


                color = {-1.f, 1.f, 1.f};
                mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
                if (color[0] != -1.f) {
                    material.MatProps &= ~(MATPROPS_USING_SPECULAR_TEXTURE);
                    material.SpecularColor = {color[0], color[1], color[2]};
                }

                mat->Get(AI_MATKEY_TEXTURE_SPECULAR(0), aitex);
                if (aitex.length) {
                    material.Textures[0] = TextureLib.Get(directory + std::string(aitex.C_Str()));
                    if (!material.Textures[2]) {
                        material.Textures[2] = Texture2D::Create(directory + std::string(aitex.C_Str()));
                        TextureLib.Add(material.Textures[2]);
                    }
                    material.MatProps |= MATPROPS_USING_SPECULAR_TEXTURE;
                }
                aitex.Clear();


                float shininess = -1.f;
                if (shininess != -1.f) {
                mat->Get(AI_MATKEY_SHININESS, shininess);
                    material.MatProps &= ~(MATPROPS_USING_SHININESS_TEXTURE);
                    material.Shininess = shininess;
                }

                mat->Get(AI_MATKEY_TEXTURE_SHININESS(0), aitex);
                if (aitex.length) {
                    material.Textures[3] = TextureLib.Get(directory + std::string(aitex.C_Str()));
                    if (!material.Textures[3]) {
                        material.Textures[3] = Texture2D::Create(directory + std::string(aitex.C_Str()));
                        TextureLib.Add(material.Textures[3]);
                    }
                    material.MatProps |= MATPROPS_USING_SHININESS_TEXTURE;
                }
                aitex.Clear();

                material.MatProps &= ~(MATPROPS_USING_DISPLACEMENT_TEXTURE);
                mat->Get(AI_MATKEY_TEXTURE_DISPLACEMENT(0), aitex);
                if (aitex.length) {
                    material.Textures[4] = TextureLib.Get(directory + std::string(aitex.C_Str()));
                    if (!material.Textures[4]) {
                        material.Textures[4] = Texture2D::Create(directory + std::string(aitex.C_Str()));
                        TextureLib.Add(material.Textures[4]);
                    }
                    material.MatProps |= MATPROPS_USING_DISPLACEMENT_TEXTURE;
                }

            }

        }
        return OxideError::OK;
    }
}