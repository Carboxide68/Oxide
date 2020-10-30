#include "Oxide/Core/Window.h"

namespace Oxide {

    const Window::WindowProps Window::CO_DEFAULT_WINDOW = {"Oxide Application", 2560, 1440, true};

    Window::Window(WindowProps props) : m_Properties(props) {
        
        Init();

    }

    void Window::Init() {

        CO_CORE_ASSERT(glfwInit(), "Couldn't initialize glfw!");

        glfwWindowHint(GLFW_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_VERSION_MINOR, 6);

        m_Window = glfwCreateWindow(m_Properties.width, m_Properties.height, m_Properties.title.c_str(), NULL, NULL);

        CO_CORE_ASSERT(m_Window, "Window not properly created!");

        glfwMakeContextCurrent(m_Window);

    }

    bool Window::BeginFrame() {

        glfwMakeContextCurrent(m_Window);
        if (glfwWindowShouldClose(m_Window)) {
            return true;
        }

        m_Renderer.BeginFrame();

        return false;
    }

    void Window::EndFrame() {
        m_Renderer.EndFrame();
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }
}