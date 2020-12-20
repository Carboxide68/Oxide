#pragma once

#include "Oxide/Core/Base.h"
#include "EventCodes.h"
#include <vector>
#include <GLFW/glfw3.h>

namespace Oxide {

    class EventHandler {
        public:

            EventHandler();

            void MouseEvent(double xPos, double yPos);
            void MouseButtonEvent(OxideEvent button, OxideEvent action);

            void KeyEvent(OxideEvent key, OxideEvent action);
            void WindowEvent(OxideEvent event);
            
            void OnMouseEvent(std::function<void (double, double)> fn);
            void OnMouseButtonEvent(std::function<void (OxideEvent, OxideEvent)> fn);
            void OnKeyEvent(std::function<void (OxideEvent, OxideEvent)> fn);
            void OnWindowEvent(std::function<void (OxideEvent)> fn);
        
        private:

            std::vector<std::function<void (double, double)>> m_MouseEvents;
            std::vector<std::function<void (OxideEvent, OxideEvent)>> m_MouseButtonEvents;
            std::vector<std::function<void (OxideEvent, OxideEvent)>> m_KeyEvents;
            std::vector<std::function<void (OxideEvent)>> m_WindowEvents;
    };

}