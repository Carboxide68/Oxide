#pragma once
#include "Oxide/Core/Base.h"
#include "Oxide/Renderer/Camera.h"
#include "Oxide/Scene/Renderable.h"
#include "Oxide/Scene/ORenderer.h"
#include "Oxide/Scene/Actor.h"
#include <unordered_map>
#include <concepts>
#include <vector>


namespace Oxide {

    template<class U, class T>
    concept Derived = std::is_base_of<T, U>::value;
    struct SceneSettings {
        int tmp;
    };

    class Scene {
    public:
        const SceneSettings& GetSettings() const;
        
        static Ref<Scene> CreateScene();
        static Ref<Scene> Create2DScene();

        template<Derived<Oxide::Actor> T>
        Ref<T> AddActor();
        template<Derived<Oxide::ORenderer> T>
        Ref<T> AddRenderer();


        Ref<Camera> camera;

    private:

        Scene(Ref<Camera> cam);
        SceneSettings m_SceneSettings;

        friend class Renderer;
        void f_Initialize();
        void f_EachFrame();
        void f_Draw(std::vector<std::pair<int, std::function<void (void)>>>& drawList);

        std::unordered_map<std::string, std::vector<Ref<Actor>>> m_Actors;
        std::unordered_map<std::string, Ref<ORenderer>> m_Renderers;

    };

    static const Ref<Scene> CO_EMPTY_SCENE = Scene::CreateScene(); //TODO: Make this something more creative
}

#include "Scene.inl"