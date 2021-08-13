#pragma once
#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/CRenderer.h"
#include "Oxide/Events/Event.h"
#include <iostream>
#define GLFW_INCLUDE_NONE
#include "GLFW/glfw3.h"

namespace Oxide {

    struct WindowSettings {
        
        std::string title;

        unsigned int width, height;
        int VSync;

    };
    
    //Each window is associated with a completely different openGL context
    class Window {
    public:

        Window(WindowSettings props = CO_DEFAULT_WINDOW);

        ~Window();

        void UpdateSettings(WindowSettings& props);

        void Init();

        bool BeginFrame(); //Returns wether the screen should be terminated or not
        void EndFrame();

        unsigned int GetHeight() const;
        unsigned int GetWidth() const;

        inline constexpr GLFWwindow* GetGLFWWindow() const {return m_Window;};


        Scope<CRenderer> renderer;
        Ref<EventHandler> eventhandler;
        static const WindowSettings CO_DEFAULT_WINDOW;

        static const Window* GetWindow(const GLFWwindow* window);

    private:

        static void error_callback(int errorCode, const char* description);

        void m_Init();
        int m_Initialized = 0;

        GLFWwindow* m_Window;
        WindowSettings m_Properties;
        static std::unordered_map<uint64_t, Window*> m_WindowInstances; //TODO: Fix this to not leak memory when destroying windows

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
        static void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);
        static void WindowResizeCallback(GLFWwindow* window, int width, int height);

    };
}