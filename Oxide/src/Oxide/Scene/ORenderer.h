#pragma once
#include "Oxide/Core/Base.h"
#include "Oxide/Scene/Renderable.h"

namespace Oxide {

    class ORenderer {
    public:
        virtual void Draw(const Ref<Renderable> renderable);
        virtual std::pair<int, std::function<void (void)>> Queue();

        std::string TypeName;

    private:

        friend class Scene;
 
    };

}