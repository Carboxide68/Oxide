#include "../Oxide/src/Oxide.h"
#include "../Oxide/src/Oxide/Core/Log.h"
#include "../Oxide/src/Oxide/Renderer/Renderer.h"
#include "../Oxide/src/Oxide/Resource/DebugCamera.h"



constexpr float data[] = {
    -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, //Back face
    -1.0f,-1.0f, 1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

    1.0f, 1.0f,-1.0f, 1.0f, 1.0f, //Left face
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f, 0.0f, 1.0f,

    1.0f,-1.0f, 1.0f, 0.0f, 0.0f, //Lower face
    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f,
    1.0f,-1.0f,-1.0f, 1.0f, 0.0f,

    1.0f, 1.0f,-1.0f, 1.0f, 1.0f, //Left face
    1.0f,-1.0f,-1.0f, 1.0f, 0.0f,
    -1.0f,-1.0f,-1.0f, 0.0f, 0.0f,

    -1.0f,-1.0f,-1.0f, 1.0f, 0.0f, //Back face
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f,-1.0f, 1.0f, 1.0f,

    1.0f,-1.0f, 1.0f, 0.0f, 0.0f, //Lower face
    -1.0f,-1.0f, 1.0f, 0.0f, 1.0f,
    -1.0f,-1.0f,-1.0f, 1.0f, 1.0f,

    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //Right face
    -1.0f,-1.0f, 1.0f, 1.0f, 0.0f,
    1.0f,-1.0f, 1.0f, 0.0f, 0.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 1.0f, //Front face
    1.0f,-1.0f,-1.0f, 0.0f, 0.0f,
    1.0f, 1.0f,-1.0f, 0.0f, 1.0f,

    1.0f,-1.0f,-1.0f, 0.0f, 0.0f, //Front face
    1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f, 1.0f, 0.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //Upper face
    1.0f, 1.0f,-1.0f, 0.0f, 0.0f,
    -1.0f, 1.0f,-1.0f, 0.0f, 1.0f,

    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, //Upper face
    -1.0f, 1.0f,-1.0f, 0.0f, 1.0f,
    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, //Right face
    -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
    1.0f,-1.0f, 1.0f, 0.0f, 0.0f
};

constexpr float vertexData[] {
    -1, -1, -1,
    -1, -1,  1,
    -1,  1, -1,
    -1,  1,  1,
    1, -1, -1,
    1, -1,  1,
    1,  1, -1,
    1,  1,  1
};

constexpr ushort indexData[] {
    0, 1, 2, //Back
    3, 2, 1,

    0, 4, 6, //Left
    2, 0, 6,

    0, 5, 1, //Under
    0, 4, 5,

    5, 7, 1, //Right
    1, 7, 3,

    4, 5, 7, //Front
    6, 4, 7,

    6, 7, 3,
    2, 6, 3

};
using namespace Oxide;
class MyObject : public Actor {
public:
    void OnStart() override {
        camera = (PerspectiveCamera*)Game->ActiveScene->camera.get();

        Game->window.renderer->SetClearColor(1, 1, 1, 1);
        Game->window.renderer->ChangeState(Oxide::CRenderer::RenderSettings::FACE_CULLING, false);
        glEnable(GL_DEPTH_TEST);
        TypeName = "MyObject";
        camera->SetFOV(60.0f);
        {
            WindowSettings settings = {"Testing", 720, 500, true};
            Game->window.UpdateSettings(settings);
        }
        Game->window.renderer->SetViewport(2560, 1440);
        camera->SetAspect(2560.0/1440.0);
        camera->SetPosition({5, 0, 5});
        camera->LookAt({0, 0, 0});
        modelMatrix = glm::mat4(1);

        shader = Shader::Create("Sandbox/resource/myShader.OS");
        texture = Texture2D::Create("Sandbox/resource/texture1.png");
        texture->SetName("texture1");

        myBuffer = Oxide::VertexBuffer::Create();
        myBuffer->BufferData(sizeof(data), (void*)data);
        myBuffer->AddElementToBufferLayout(OxideType::Float, 3);
        myBuffer->AddElementToBufferLayout(OxideType::Float, 2);
    }

    void EachFrame() override {
        shader->Bind();
        shader->SetUniform("color", glm::vec3(1, 1, 1));
        shader->SetUniform("compositeMatrix", camera->GetPerspectiveMatrix() * camera->GetViewMatrix() * modelMatrix);
        shader->SetUniform("modelMatrix", modelMatrix);
        texture->Bind(0);
        shader->SetUniform(texture->GetName(), 0);

        myBuffer->DrawArrays(36);
    }

    Ref<Oxide::VertexBuffer> myBuffer;
    glm::mat4 modelMatrix;
    PerspectiveCamera* camera;
    Ref<Shader> shader;
    Ref<Texture2D> texture;

    ACTOR_ESSENTIALS(MyObject);

};

int main(int argc, char *argv[]) {

    Oxide::Log::Init();
    Oxide::Log::GetCoreLogger()->set_level(spdlog::level::info);
    Oxide::Log::GetClientLogger()->set_level(spdlog::level::info);

    Game->ActiveScene->AddActor<MyObject>();
    Game->ActiveScene->AddActor<DebugCamera>();
    Game->Start();

}