#include "Oxide/Core/Window.h"
#include "Oxide/Core/Log.h"
#include "Oxide/Events/Event.h"

namespace Oxide {

    const Window::WindowProps Window::CO_DEFAULT_WINDOW = {"Oxide Application", 2560, 1440, true};
    std::vector<Window*> Window::m_WindowInstances;

    Window::Window(WindowProps props) : m_Properties(props) {
        
        Init();

    }

    Window::~Window() {

        glfwDestroyWindow(m_Window);

    }

    void Window::Init() {

        if(!glfwInit()) {
            CO_ERROR("Couldn't initialize glfw!\n");
            return;
        }

        eventhandler = CreateRef<EventHandler>();
        glfwSetErrorCallback(error_callback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        m_Window = glfwCreateWindow(m_Properties.width, m_Properties.height, m_Properties.title.c_str(), NULL, NULL);

        if (!m_Window) {
            const char* desc;
            int err = glfwGetError(&desc);
            CO_ERROR("Window didn't initialize correctly! Error: %s\n", desc);
        }
        glfwMakeContextCurrent(m_Window);

        if (m_Properties.VSync == true) {
            glfwSwapInterval(1);
        }

        renderer = Renderer::Create();
        renderer->Init();

        renderer->SetViewport(0, 0, m_Properties.width, m_Properties.height);
        m_WindowInstances.push_back(this);
        glfwSetKeyCallback(m_Window, KeyCallback);
        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
        glfwSetCursorPosCallback(m_Window, CursorPosCallback);
    }

    bool Window::BeginFrame() {

        if (glfwWindowShouldClose(m_Window)) {
            return true;
        }

        renderer->BeginFrame();

        return false;
    }

    void Window::EndFrame() {
        renderer->EndFrame();
        glfwSwapBuffers(m_Window);
        glfwPollEvents();
    }

    void Window::error_callback(int errorCode, const char* description) {
        CO_ERROR("Something went wrong with glfw!\nError: %s\nError code: %d\n", description, errorCode);
        return;
    }

    uint Window::GetHeight() const {
        return m_Properties.height;
    }

    uint Window::GetWidth() const {
        return m_Properties.width;
    }

    const Window* Window::GetWindow(const GLFWwindow* window) {
        for (Window* testWindow : m_WindowInstances) {
            if (window == testWindow->GetGLFWWindow()) {
                return testWindow;
            }
        }
        return nullptr;
    }

    void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        GetWindow(window)->eventhandler->KeyEvent((OxideEvent)GLFWToOxideEvent[key], (OxideEvent)GLFWToOxideEvent[action]); 
    }

    void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        GetWindow(window)->eventhandler->MouseButtonEvent((OxideEvent)GLFWToOxideEvent[button], (OxideEvent)GLFWToOxideEvent[action]);
    }

    void Window::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        GetWindow(window)->eventhandler->MouseEvent(xpos, ypos);
    }
}