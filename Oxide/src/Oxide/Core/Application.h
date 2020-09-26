#include "Oxide/Core/Base.h"

namespace Oxide {

    class Application {

        Application(const std::string& name = "Monoxide");
        virtual ~Application() = default;

        Window& GetWindow();

    };

}