#pragma once

#include "Oxide/Core/Base.h"

#define ACTOR_ESSENTIALS(__ClassName__) private: friend Oxide::Scene; __ClassName__(){return;}

namespace Oxide {

    class Scene;

    class Actor {
    public:

        virtual void OnStart() = 0;
        virtual void EachFrame() = 0;

        std::string TypeName;
    
    protected:
        friend Oxide::Scene;
        Actor() {return;}

    };

}