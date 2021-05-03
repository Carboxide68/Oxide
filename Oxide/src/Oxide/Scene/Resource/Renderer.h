#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Resource/Renderable.h"
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


    /**
     * 
     * An interface class that specifies the requirements for a renderer
     * 
     */
    class Renderer {

    public:

        virtual void Draw();

        std::string Name;

    protected:

        virtual void DrawAll() const;

    };

    template<class T>
    class RendererTemplate : public Renderer {
    public:

        virtual void Draw();
    
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

    class CO_STANDARD_RENDERER : public RendererTemplate<StandardRenderData> { //TODO: Make a better way to create these classes
    
    public:

        CO_STANDARD_RENDERER();

        static Ref<Renderer> Create();

        void Draw() override;

        void Load(const StandardRenderData data) override;

    protected:

        void DrawAll() const override;

    private:

        Ref<VertexBuffer> m_VB;
        Ref<IndexBuffer> m_IB;
        Ref<Shader> m_Shader;


    };
}