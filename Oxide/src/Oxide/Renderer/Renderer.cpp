#include "Oxide/Renderer/Renderer.h"
#include <algorithm>

namespace Oxide {


    void Renderer::Start() {

        window.eventhandler->OnKeyEvent ([&] (OxideEvent key, OxideEvent action) -> void {
            if (key == OxideEvent::KEY_ESCAPE && action == OxideEvent::Released) {
                glfwSetWindowShouldClose(window.GetGLFWWindow(), true);
            }
        });

        window.eventhandler->OnResizeEvent([&] (int width, int height) -> void {
            window.renderer->SetViewport(width, height);
            ActiveScene->camera->SetAspect((float)width/(float)height);
        });

        m_ChangeScene();
        while (window.BeginFrame()) {
            m_ActiveScene->f_EachFrame();
            m_ActiveScene->f_Draw(m_Draws);

            std::sort(m_Draws.begin(), m_Draws.end(), [](auto first, auto other) -> bool {return first.first < other.first;});
            for (auto draw : m_Draws){ 
                draw.second();
            }

            window.EndFrame();
            FrameMark
            if (m_ShouldChangeScene) {
                m_ChangeScene();
                m_ShouldChangeScene = 0;
            }
            m_Draws.clear();
        }
    }

    void Renderer::ChangeScene() {
        m_ShouldChangeScene = 1;
    }
    
    void Renderer::m_ChangeScene() {
        m_ActiveScene = ActiveScene;
        m_ActiveScene->f_Initialize();
    }

    Scope<Renderer> Renderer::Create() {
        return Scope<Renderer>(new Renderer());
    }

    void Renderer::Init() {
        window.Init();
        window.UpdateSettings(WindowSet);
    }

    Renderer::Renderer() {
        m_Draws.reserve(100);
        ActiveScene = Scene::CreateScene();
        m_ActiveScene = ActiveScene;
        WindowSet = m_LoadWindowSettings();
        crenderer = window.renderer.get();
    }

}