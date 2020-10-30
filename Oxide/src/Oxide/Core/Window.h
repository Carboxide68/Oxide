#pragma once
#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Renderer.h"
#include "GLFW/glfw3.h"

namespace Oxide {

    //Each window is associated with a completely different openGL context
    class Window {
    public:

        struct WindowProps {
            
            std::string title;

            unsigned int height, width;
            bool VSync;

        };

        Window(WindowProps props = CO_DEFAULT_WINDOW);

        bool BeginFrame(); //Returns wether the screen should be terminated or not
        void EndFrame();

        unsigned int GetHeight() const;
        unsigned int GetWidth() const;

        GLFWwindow* GetWindow();

        static const WindowProps CO_DEFAULT_WINDOW;

    private:

        void Init();

        GLFWwindow* m_Window;
        WindowProps m_Properties;
        Renderer m_Renderer;

    };

}