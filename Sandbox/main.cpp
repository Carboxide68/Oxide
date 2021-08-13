#include "../Oxide/src/Oxide.h"
#include "../Oxide/src/Oxide/Core/Log.h"
#include "../Oxide/src/Oxide/Renderer/Renderer.h"
#include "../Oxide/src/Oxide/Scene/MeshRenderer.h"
#include "../Oxide/src/Oxide/Resource/DebugCamera.h"
#include "../Oxide/src/Oxide/Scene/Model.h"

using namespace Oxide;
class MyObject : public Actor {
public:
    void OnStart() override {
        PerspectiveCamera* camera = (PerspectiveCamera*)Game->scene->camera.get();
        myRenderer = std::static_pointer_cast<MeshRenderer>(Game->scene->GetRenderer("MeshRenderer"));

        myModel.LoadModel("Sandbox/resource/backpack/scene.gltf");
        for (auto& mesh : myModel.Meshes) {
            mesh.SetScale(0.39f);
        }
        myIds.resize(myModel.Meshes.size(), 0);

        Game->window.renderer->SetClearColor(1, 1, 1, 1);
        Game->crenderer->Disable(GL_CULL_FACE);
        Game->crenderer->Enable(GL_DEPTH_TEST);
        camera->SetFOV(60.0f);
        
        WindowSettings settings = {"Testing", 720, 500, true};
        Game->window.UpdateSettings(settings);
    
        Game->window.renderer->SetViewport(2560, 1440);
        camera->SetAspect(2560.0/1440.0);
        camera->SetPosition({2, 0, 2});
        camera->LookAt({0, 0, 0});
        
    }

    void EachFrame() override {
        for (uint i = 0; i < myModel.Meshes.size(); i++) {
            myRenderer->Draw(myIds[i], myModel.Meshes[i], false);
        }
    }

    const std::string TypeName = "MyObject";

    Ref<MeshRenderer> myRenderer;
    std::vector<uint> myIds;
    Model myModel;

    ACTOR_ESSENTIALS(MyObject);

};

int main(int argc, char *argv[]) {

    Oxide::Log::Init();
    Oxide::Log::GetCoreLogger()->set_level(spdlog::level::info);
    Oxide::Log::GetClientLogger()->set_level(spdlog::level::info);

    Game->Init();

    Game->scene->AddRenderer<MeshRenderer>();

    Game->scene->AddActor<MyObject>();
    Game->scene->AddActor<DebugCamera>();
    Game->Start();

}