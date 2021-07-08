#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Resource/Renderer.h"

namespace Oxide {
    class Renderer;

    class Renderable {
    public:

        Ref<Renderer> ObjectRenderer;
        
        virtual inline void Draw() const {ObjectRenderer->Draw(this);}

        inline const std::string GetRendererName() const {return m_RendererName;}

    private:

        std::string m_RendererName;

    };
}