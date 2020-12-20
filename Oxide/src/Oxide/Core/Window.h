#pragma once
#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Renderer.h"
#include "Oxide/Events/Event.h"
#include <iostream>
#define GLFW_INCLUDE_NONE
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

        ~Window();

        bool BeginFrame(); //Returns wether the screen should be terminated or not
        void EndFrame();

        unsigned int GetHeight() const;
        unsigned int GetWidth() const;

        inline constexpr GLFWwindow* GetGLFWWindow() const {return m_Window;};


        Scope<Renderer> renderer;
        Ref<EventHandler> eventhandler;
        static const WindowProps CO_DEFAULT_WINDOW;

        static const Window* GetWindow(const GLFWwindow* window);

    private:

        static void error_callback(int errorCode, const char* description);

        void Init();

        GLFWwindow* m_Window;
        WindowProps m_Properties;
        static std::vector<Window*> m_WindowInstances; //TODO: Fix this to not leak memory when destroying windows

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

    };
}