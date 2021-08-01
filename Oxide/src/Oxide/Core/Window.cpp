#include "Oxide/Core/Window.h"
#include "Oxide/Core/Log.h"
#include "Oxide/Events/Event.h"

namespace Oxide {

    const WindowSettings Window::CO_DEFAULT_WINDOW = {"Oxide Application", 2560, 1440, true};

    Window::Window(WindowSettings props) {
        m_Properties = props;
    }

    void Window::Init() {
        if (!m_Initialized)
            m_Init();
        else
            CO_ERROR("Window has already been initialized!\n");
        return;
    }

    Window::~Window() {

        glfwDestroyWindow(m_Window);

    }

    void Window::UpdateSettings(WindowSettings& settings) {
        m_Properties = settings;
        glfwSetWindowTitle(m_Window, settings.title.c_str());
        glfwSetWindowSize(m_Window, settings.width, settings.height);
        glfwSwapInterval((settings.VSync) ? 1 : 0);
    }

    void Window::m_Init() {

        if(!glfwInit()) {
            CO_ERROR("Couldn't initialize glfw!\n");
            return;
        }

        glfwSetErrorCallback(error_callback);

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        #ifdef DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        #endif

        m_Window = glfwCreateWindow(m_Properties.width, m_Properties.height, m_Properties.title.c_str(), NULL, NULL);


        if (!m_Window) {
            const char* desc;
            glfwGetError(&desc);
            CO_ERROR("Window didn't initialize correctly! Error: %s\n", desc);
            CO_CORE_ASSERT(false, "");
        }
        glfwMakeContextCurrent(m_Window);

        m_WindowInstances[(uint64_t)m_Window] = this;
        eventhandler = CreateRef<EventHandler>();

        if (m_Properties.VSync == true) {
            glfwSwapInterval(1);
        }

        renderer = CRenderer::Create();
        renderer->Init();

        renderer->SetViewport(0, 0, m_Properties.width, m_Properties.height);
        glfwSetKeyCallback(m_Window, KeyCallback);
        glfwSetMouseButtonCallback(m_Window, MouseButtonCallback);
        glfwSetCursorPosCallback(m_Window, CursorPosCallback);
        glfwSetWindowSizeCallback(m_Window, WindowResizeCallback);
        m_Initialized = true;
    }

    bool Window::BeginFrame() {

        if (glfwWindowShouldClose(m_Window)) {
            return false;
        }

        glfwPollEvents();
        renderer->BeginFrame();

        return true;
    }

    void Window::EndFrame() {
        renderer->EndFrame();
        glfwSwapBuffers(m_Window);
    }

    void Window::error_callback(int errorCode, const char* description) {
        CO_CORE_ERROR("Something went wrong with glfw!\nError: %s\nError code: %d\n", description, errorCode);
        return;
    }

    uint Window::GetHeight() const {
        return m_Properties.height;
    }

    uint Window::GetWidth() const {
        return m_Properties.width;
    }

    const Window* Window::GetWindow(const GLFWwindow* window) {
        if (m_WindowInstances.find(((uint64_t)window)) == m_WindowInstances.end()) return nullptr;
        return m_WindowInstances[(uint64_t)window];
    }

    void Window::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
        GetWindow(window)->eventhandler->KeyEvent((OxideEvent)KeyEventGLFWToOxide[key], (OxideEvent)ActionGLFWToOxide[action]); 
    }

    void Window::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
        GetWindow(window)->eventhandler->MouseButtonEvent((OxideEvent)MouseButtonGLFWToOxide[button], (OxideEvent)ActionGLFWToOxide[action]);
    }

    void Window::CursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
        GetWindow(window)->eventhandler->MouseEvent(xpos, ypos);
    }

    void Window::WindowResizeCallback(GLFWwindow* window, int width, int height) {
        GetWindow(window)->eventhandler->ResizeEvent(width, height);
    }
}