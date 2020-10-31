#include "Oxide/Core/Window.h"

namespace Oxide {

    const Window::WindowProps Window::CO_DEFAULT_WINDOW = {"Oxide Application", 2560, 1440, true};

    Window::Window(WindowProps props) : m_Properties(props) {
        
        Init();

    }

    Window::~Window() {

        glfwDestroyWindow(m_Window);

    }

    void Window::Init() {

        if(!glfwInit()) {
            printf("Couldn't initialize glfw!\n");
            return;
        }

        glfwSetErrorCallback(error_callback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Properties.width, m_Properties.height, m_Properties.title.c_str(), NULL, NULL);

        if (!m_Window) {
            const char* desc;
            int err = glfwGetError(&desc);
            printf("Window didn't initialize correctly! Error: %s\n", desc);
        }
        glfwMakeContextCurrent(m_Window);

        if (m_Properties.VSync == true) {
            glfwSwapInterval(1);
        }

        renderer.Init();

        renderer.SetViewport(0, 0, m_Properties.width, m_Properties.height);

    }

    bool Window::BeginFrame() {

        if (glfwWindowShouldClose(m_Window)) {
            return true;
        }

        renderer.BeginFrame();

        return false;
    }

    void Window::EndFrame() {
        renderer.EndFrame();
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void Window::error_callback(int errorCode, const char* description) {
        printf("Something went wrong with glfw!\nError: %s\nError code: %d\n", description, errorCode);
        return;
    }

    uint Window::GetHeight() const {
        return m_Properties.height;
    }

    uint Window::GetWidth() const {
        return m_Properties.width;
    }

}