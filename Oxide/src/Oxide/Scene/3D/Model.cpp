
#include "Oxide/Core/Log.h"
#include "Oxide/Scene/3D/Model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Oxide {
    
    OxideError Model::LoadModel(const std::string& filePath) {

        Meshes.clear();

        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filePath,
            aiProcess_CalcTangentSpace      |
            aiProcess_Triangulate           |
            aiProcess_JoinIdenticalVertices |
            aiProcess_SortByPType);
        
        if (!scene) {
            CO_CORE_WARN("Failed to load model at path: %s!", filePath);
            return OxideError::Error;
        }

        Name = std::string(scene->mName.C_Str());
        
        for (uint i = 0; i < scene->mNumMeshes; i++) {

            Mesh curMesh;
            const aiMesh* curaiMesh = scene->mMeshes[i]; 
                        
            //TODO: Make this more capable
            curMesh.VertexData = std::vector<std::array<float, 3>>(&curaiMesh->mVertices[0], &curaiMesh->mVertices[curaiMesh->mNumVertices - 1]);
            
            for (uint j = 0; j < curaiMesh->mNumFaces; j++) {
                curMesh.Faces.push_back(std::vector<unsigned int>(curaiMesh->mFaces->mIndices, curaiMesh->mFaces->mNumIndices + curaiMesh->mFaces->mIndices));
            }

            curMesh.SetUsingNormals(curaiMesh->HasNormals());
            if (curMesh.GetUsingNormals()) {
                curMesh.Normals = std::vector<std::array<float, 3>>(&curaiMesh->mNormals[0], &curaiMesh->mNormals[0] + curaiMesh->mNumVertices);
            }
            Meshes.push_back(curMesh);
        }
        return OxideError::OK;
    }
}