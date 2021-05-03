#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Resource/Renderable.h"
#include "Oxide/Scene/3D/Mesh.h"

namespace Oxide {

    class Model : Renderable {
    public:

        Model();

        OxideError LoadModel(const std::string& filePath);

        std::vector<Mesh> Meshes;
        
        std::string Name;

    private:

    };
}