#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Resource/Uniform.h"
#include "Oxide/Scene/Resource/Material.h"
#include "Oxide/Renderer/Texture.h"
#include "Oxide/Renderer/Buffer.h"
#include "Oxide/Renderer/Shader.h"
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

namespace Oxide {

    class Renderable;

    /**
     * 
     * An interface class that specifies the requirements for a renderer
     * 
     */
    class Renderer {

    public:

        virtual void Draw(const Renderable *object);

        static const std::string Name;

    protected:

        virtual void DrawAll() const;

    };

    template<class T>
    class RendererTemplate : public Renderer {
    public:

        virtual void Draw(const Renderable *object);
    
        virtual void Load(const T data);
    
    private:

        virtual void DrawAll() const;

    };

    struct StandardRenderData {

        std::vector<float> *vertices;
        std::vector<float> *normals = {};
        std::vector<float> *texCoords = {};
        std::vector<unsigned short> *indices = {};

        Uniform<glm::mat4> modelMatrix;
        Material material;
    };
}