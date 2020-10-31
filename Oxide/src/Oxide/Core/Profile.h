#pragma once
#include "Oxide/Core/Base.h"

namespace Oxide {

    class Profiler {

    public:

        static void Init();



    private:

    };

}

#ifdef PROFILE
#define CO_CORE_PROFILE_FUNC
#define CO_PROFILE_FUNC
#else
#define CO_CORE_PROFILE_FUNC
#define CO_PROFILE_FUNC
#endif