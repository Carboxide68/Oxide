
#include "Oxide/Renderer/TextureSettings.h"

namespace Oxide {

    const TextureSettings TextureSettings::STANDARD_SETTING = TextureSettings();

    TextureSettings::TextureSettings() {
        TextureSettings(Wrap::REPEAT, Mag::LINEAR, Min::NEAREST);
    }

    TextureSettings::TextureSettings(Wrap wrap, Mag mag, Min min) {
        WrapType = wrap;
        MagFilter = mag;
        MinFilter = min;
        Mipmap = 0;
    };


}