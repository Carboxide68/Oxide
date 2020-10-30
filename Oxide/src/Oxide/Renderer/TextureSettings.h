#pragma once

#include "Oxide/Core/Base.h"


namespace Oxide {

    class TextureSettings {

    public:
        enum class Wrap {
            NONE = 0,
            REPEAT = 1,
            CLAMP = 2
        };

        enum class Mag {
            NONE = 0,
            NEAREST = 1,
            LINEAR = 2
        };

        enum class Min {
            NONE = 0,
            LINEAR = 1,
            NEAREST = 2
        };

        TextureSettings();
        TextureSettings(Wrap wrap, Mag mag, Min min);

        Wrap WrapType;
        Mag MagFilter;
        Min MinFilter;
        int Mipmap;

        static const TextureSettings STANDARD_SETTING;

    };

}