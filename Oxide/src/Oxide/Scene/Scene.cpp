#include "Oxide/Scene/Scene.h"

namespace Oxide {

    const SceneSettings& Scene::GetSettings() const {
        return m_SceneSettings;
    }

    Ref<Scene> Scene::CreateScene() {
        return Ref<Scene>(new Scene(PerspectiveCamera::Create()));
    }

    Ref<Scene> Scene::Create2DScene() {
        return Ref<Scene>(new Scene(OrthographicCamera::Create()));
    }

    Scene::Scene(Ref<Camera> cam) {
        camera = cam;
    }

    void Scene::f_Initialize() {
        ZoneScoped
        for (auto actorType : m_Actors) {
            for (auto actor : actorType.second){
                if (actor == nullptr) continue;
                actor->OnStart();
            }
        }
    }

    void Scene::f_EachFrame() {
        ZoneScoped
        for (auto actorType : m_Actors) {
            for (auto actor : actorType.second){
                if (actor == nullptr) continue;
                actor->EachFrame();
            }
        }
    }

    void Scene::f_Draw(std::vector<std::pair<int, std::function<void (void)>>>& drawList) {
        ZoneScoped
        for (auto renderer : m_Renderers) {
            if (renderer.second == nullptr) continue;
            auto newdraws = renderer.second->Queue();
            drawList.insert(drawList.end(), newdraws.begin(), newdraws.end());
        }
    }

    std::vector<Ref<Actor>> Scene::GetActors(const std::string& name) {
        return m_Actors[name];
    }

    Ref<ORenderer> Scene::GetRenderer(const std::string& name) {
        return m_Renderers[name];
    }
}