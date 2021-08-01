#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Texture.h"
#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <vector>

#define MATPROPS_USING_AMBIENT_TEXTURE (1 << 0)
#define MATPROPS_USING_DIFFUSE_TEXTURE (1 << 1)
#define MATPROPS_USING_SPECULAR_TEXTURE (1 << 2)
#define MATPROPS_USING_SHININESS_TEXTURE (1 << 3)
#define MATPROPS_USING_DISPLACEMENT_TEXTURE (1 << 4)

namespace Oxide {
    class Shader;

    struct MeshMaterial {
        std::array<Ref<Texture2D>, 5> Textures{{NULL,NULL,NULL,NULL,NULL}}; //0: Ambient, 1: Diffuse, 2: Specular, 3: Shininess, 4: DisplacementMap;

        glm::vec3 DiffuseColor = glm::vec3(0.0f);
        glm::vec3 AmbientColor = glm::vec3(0.0f);
        glm::vec3 SpecularColor = glm::vec3(0.0f);

        float Shininess = 0.0f;

        char MatProps = 0;

        void SetUniforms(Ref<Shader> shader);
    };

    /**
     * @brief Class for storing mesh data.
     * 
     */
    class Mesh {
        public:

            Mesh();
            
            /**
             * @brief Get data in raw form
             * 
             * @param size The byte size of the data
             * @param weave If you want the data to be aligned like ABCABCABCABC or AAAABBBBCCCC. Default is woven.
             * @return void* Pointer to the data, allocated on the heap. This must use free(data) at the end of it's lifetime.
             */
            std::vector<float> GetVertexData(uint &size, bool weave = true) const;
            std::vector<uint> GetIndexData(uint &size) const;

            void SetScale(const float& scale);

            void UpdateMesh();

            std::vector<std::array<float, 3>> Vertices;
            std::vector<std::array<float, 3>> Normals;
            std::vector<std::array<float, 2>> TexCoords;

            std::vector<std::array<uint, 3>> Faces;

            bool UsingNormals;
            bool UsingTexCoords;
            bool UsingIndices;

            MeshMaterial Material;
            glm::mat4 ModelMatrix;

        private:

    };
}