#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Scene.h"
#include "Oxide/Renderer/Buffer.h"
#include "Oxide/Renderer/Shader.h"
#include "Oxide/Scene/Mesh.h"

namespace Oxide {

    /**
     * @brief Standard renderer for meshes.
     * 
     * Should only by created as part of a scene.
     * Read documentation for function Draw() for usage.
     * 
     */
    class MeshRenderer : public ORenderer {
    public:

        /**
         * @brief Add a mesh to draw for the current frame.
         * 
         * @param id Unique id. `-1` to generate a new id. This value may be changed by the function.
         * @param mesh The mesh you want to draw.
         * @param update If the mesh has been updated since last draw call, set this to true. Standard is false.
         */
        void Draw(uint &id, const Mesh& mesh, bool update = false);

        /**
         * @brief Warning! Not fully implemented yet!
         * 
         */
        void Destroy(uint &id); //TODO: Implement this

        std::vector<std::pair<int, std::function<void (void)>>> Queue() override;

        inline float GetMinPercentPopulated() {return m_MinPercentPopulated;}
        inline void SetMinPercentPopulated(const float percent) {m_MinPercentPopulated = percent;}

        const std::string TypeName = "MeshRenderer"; /* TypeName, this is name that the scene locates this class */

    private:

        struct DrawObject {
            MeshMaterial Material;

            std::pair<uint, uint> VertexBufferLocation {0, 0};
            std::pair<uint, uint> IndexBufferLocation {0, 0};
            glm::mat4 ModelMatrix = glm::mat4(1);
            char DrawMode = 1; /* 0: Indexed; 1: Arrays */
            uint DrawCount = 0;
            char DataProps = 0;
        };

        MeshRenderer(Scene* scene);

        CleanBuffers();

        Ref<VertexBuffer> m_VertexBuffer;
        Ref<IndexBuffer> m_IndexBuffer;
        Ref<Shader> m_Shader;

        std::unordered_map<uint, DrawObject> m_IdLookup;
        std::vector<uint> m_Draws;
        uint m_IdTally = 1;
        size_t m_VertexbufferHead = 0;
        size_t m_IndexbufferHead = 0;

        float m_MinPercentPopulated = 0.67f;

        friend Scene;

    };

}