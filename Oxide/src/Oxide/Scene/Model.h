#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Mesh.h"
#include "Oxide/Renderer/Texture.h"
#include <assimp/Importer.hpp>


namespace Oxide {

    class Model {
    public:

        OxideError LoadModel(const std::string& filePath);

        std::vector<Mesh> Meshes;
        
        std::string Name;

    private:

        static Assimp::Importer importer;

    };
}