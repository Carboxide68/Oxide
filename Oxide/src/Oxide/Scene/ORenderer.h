#pragma once
#include "Oxide/Core/Base.h"

#define RENDERER_ESSENTIALS(__ClassName__) private: friend Oxide::Scene; __ClassName__(){return;}

namespace Oxide {

    class Scene;

    class ORenderer {
    public:
        virtual std::vector<std::pair<int, std::function<void (void)>>> Queue() = 0;

        const std::string TypeName;

    protected:

        Scene* m_ParentScene;
        friend Scene;
        ORenderer(Scene* parentScene) : m_ParentScene(parentScene){return;}

    };

}