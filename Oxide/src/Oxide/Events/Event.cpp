#include "Oxide/Events/Event.h"

namespace Oxide {

    EventHandler::EventHandler() {

    }

    void EventHandler::MouseEvent(double x, double y) {

        for (auto i : m_MouseEvents) {
            i(x, y);
        }

    }

    void EventHandler::MouseButtonEvent(OxideEvent button, OxideEvent action) {

        for (auto i : m_MouseButtonEvents) {
            i(button, action);
        }

    }
    void EventHandler::KeyEvent(OxideEvent key, OxideEvent action) {

        for (auto i : m_KeyEvents) {
            i(key, action);
        }

    }

    void EventHandler::WindowEvent(OxideEvent event) {

        for (auto i : m_WindowEvents) {
            i(event);
        }
    }

    void EventHandler::ResizeEvent(int width, int height) {
        for (auto i : m_ResizeEvents) {
            i(width, height);
        }
    }

    void EventHandler::OnMouseEvent(std::function<void (double, double)> fn) {
        m_MouseEvents.push_back(fn);
    }

    void EventHandler::OnMouseButtonEvent(std::function<void (OxideEvent, OxideEvent)> fn) {
        m_MouseButtonEvents.push_back(fn);
    }

    void EventHandler::OnKeyEvent(std::function<void (OxideEvent, OxideEvent)> fn) {
        m_KeyEvents.push_back(fn);
    }

    void EventHandler::OnWindowEvent(std::function<void (OxideEvent)> fn) {
        m_WindowEvents.push_back(fn);
    }

    void EventHandler::OnResizeEvent(std::function<void (int,int)> fn) {
        m_ResizeEvents.push_back(fn);
    }

    // void EventHandler::GLFWWindowEvent(GLFWwindow *window, int key, int scancode, int action, int mods) {

    // }

}