#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Resource/Material.h"
#include <vector>

namespace Oxide {

    class Mesh {
        public:

            Mesh();
            ~Mesh();

            void SetUsingNormals(bool state);
            bool GetUsingNormals();

            void AssociateMaterial(Ref<Material> material);

            void UpdateMesh();

            std::vector<std::array<float, 3>> VertexData;
            std::vector<std::vector<unsigned int>> Faces;
            std::vector<std::array<float, 3>> Normals;

        private:

            Ref<Material> m_AssociatedMaterial;
            bool m_UsingNormals;
    };
}