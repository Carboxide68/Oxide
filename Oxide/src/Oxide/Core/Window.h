#pragma once

#include "Oxide/Core/Base.h"
#include "glfw/glfw3.h"

namespace Oxide {

    struct WindowProps {
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Oxide",
			        uint32_t width = 1280,
			        uint32_t height = 720)
			: Title(title), Width(width), Height(height){}
	};

    class Window {
    public:

        Window(const WindowProps& props = WindowProps());
        ~Window();

        void OnUpdate();

        uint32_t GetWidth() const;
        uint32_t GetHeight() const;

        void SetVSync(bool enabled);
        bool IsVSync() const;

        void* GetNativeWindow() const;

    private:

        GLFWwindow* m_Window;

        struct WindowData {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
        };

    }

}