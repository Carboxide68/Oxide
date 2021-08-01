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
        PerspectiveCamera* camera = (PerspectiveCamera*)Game->ActiveScene->camera.get();
        myRenderer = std::static_pointer_cast<MeshRenderer>(Game->ActiveScene->GetRenderer("MeshRenderer"));

        myModel.LoadModel("Sandbox/resource/North_American_SNJ_Harvard_USN_V08.obj");
        for (auto& mesh : myModel.Meshes) {
            mesh.SetScale(0.039f);
        }
        myIds.resize(myModel.Meshes.size(), 0);

        Game->window.renderer->SetClearColor(1, 1, 1, 1);
        Game->window.renderer->ChangeState(Oxide::CRenderer::RenderSettings::FACE_CULLING, false);
        glEnable(GL_DEPTH_TEST);
        camera->SetFOV(60.0f);
        {
            WindowSettings settings = {"Testing", 720, 500, false};
            Game->window.UpdateSettings(settings);
        }
        Game->window.renderer->SetViewport(2560, 1440);
        camera->SetAspect(2560.0/1440.0);
        camera->SetPosition({5, 0, 5});
        camera->LookAt({0, 0, 0});
        
    }

    void EachFrame() override {
        for (uint i = 0; i < myModel.Meshes.size(); i++) {
            myRenderer->Draw(myIds[i], myModel.Meshes[i]);
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

    Game->ActiveScene->AddRenderer<MeshRenderer>();

    Game->ActiveScene->AddActor<MyObject>();
    Game->ActiveScene->AddActor<DebugCamera>();
    Game->Start();

}