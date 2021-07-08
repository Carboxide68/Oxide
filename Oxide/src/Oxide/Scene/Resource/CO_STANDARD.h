
#include "Oxide/Core/Base.h"
#include "Oxide/Core/Log.h"
#include "Oxide/Scene/Resource/Renderer.h"
#include "Oxide/Scene/Resource/Renderable.h"
    
namespace Oxide {
    class CO_STANDARD_RENDERABLE : public Renderable {
    public:

        virtual const StandardRenderData GetStandardRenderData();


    };

    class CO_STANDARD_RENDERER : public RendererTemplate<StandardRenderData> { //TODO: Make a better way to create these classes
    
    public:

        CO_STANDARD_RENDERER();
        ~CO_STANDARD_RENDERER();

        static Ref<Renderer> Create();

        void Draw(const Renderable *object) override;

        void Load(const StandardRenderData data) override;

        static const std::string Name;

    protected:

        void DrawAll() const override;

    private:

        Ref<VertexBuffer> m_VB;
        Ref<IndexBuffer> m_IB;
        Ref<Shader> m_Shader;
        std::vector<UniformShell*> m_Uniforms;

    };
}