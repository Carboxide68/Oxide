#pragma once

#include "Oxide/Core/Base.h"
#include "Oxide/Core/Window.h"
#include "Oxide/Scene/Scene.h"

namespace Oxide {

    class Renderer {
    public:


        void Start();
        void ChangeScene(); //There's a private member that is the scene that it's actually working on. This makes the user able to make ActiveScene point to something new, and then call
                            //ChangeScene to update things accordingly i.e calling the initialization functions etc.

        /**
         * @brief Initialize window etc. Must be ran before anything else
         * 
         * 
         */
        void Init();

        static Scope<Renderer> Create();

        Ref<Scene> scene;
        SceneSettings sceneSet;
        WindowSettings windowSet;
        CRenderer* crenderer;
        Window window;

    private:

        Renderer();

        Ref<Scene> m_ActiveScene;
        void m_ChangeScene();
        int m_ShouldChangeScene = 0;

        
        void m_MainLoop();
        const WindowSettings& m_LoadWindowSettings() {return Window::CO_DEFAULT_WINDOW;};
        std::vector<std::pair<int, std::function<void (void)>>> m_Draws;

    };
    extern Scope<Renderer> Game;

}
using Oxide::Game;