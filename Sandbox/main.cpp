#include "../Oxide/src/Oxide.h"
#include "../Oxide/src/Oxide/Core/Log.h"

int main(int argc, char *argv[]) {

    Oxide::Log::Init();
    Oxide::Log::GetCoreLogger()->set_level(spdlog::level::info);
    Oxide::Log::GetClientLogger()->set_level(spdlog::level::info);

    Oxide::Window myWindow({"First application", 720, 500, true});
    myWindow.renderer->SetClearColor(1, 1, 1, 1);
    myWindow.renderer->ChangeState(Oxide::Renderer::RenderSettings::FACE_CULLING, false);

    Oxide::Ref<Oxide::VertexBuffer> myBuffer = Oxide::VertexBuffer::Create();

    Oxide::Camera myCamera;
    myCamera.SetFOV(70.0f);
    myCamera.SetAspect((float)myWindow.GetWidth()/(float)myWindow.GetHeight());
    myWindow.renderer->SetViewport(720, 500);
    float angle = 0;
    myCamera.SetPosition(glm::vec3(cos(angle) * 5, 0, sin(angle) * 5));

    Oxide::Ref<Oxide::Shader> myShader = Oxide::Shader::Create("Sandbox/shader/myShader.OS");

    float data[] = {
        -1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,

        1.0f, 1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,

        1.0f, 1.0f,-1.0f,
        1.0f,-1.0f,-1.0f,
        -1.0f,-1.0f,-1.0f,

        -1.0f,-1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        -1.0f,-1.0f,-1.0f,

        -1.0f, 1.0f, 1.0f,
        -1.0f,-1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f,-1.0f,

        1.0f,-1.0f,-1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f,-1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f,-1.0f,
        -1.0f, 1.0f, 1.0f,

        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f,-1.0f, 1.0f
    };

    glm::mat4 myModelMatrix(1);
    myModelMatrix[0][0] = 1;
    myModelMatrix[1][1] = 1;
    myModelMatrix[2][2] = 1;

    myBuffer->BufferData(sizeof(data), (void*)data);
    myBuffer->AddElementToBufferLayout(Oxide::OxideType::Float, 3);

    bool moving = true;
    myWindow.eventhandler->OnMouseButtonEvent([&] (Oxide::OxideEvent button, Oxide::OxideEvent action) -> void {
        if (action == Oxide::OxideEvent::MOUSE_BUTTON_6 && button == Oxide::OxideEvent::MOUSE_BUTTON_5) {
            moving = !moving;
        }
    });

    while (!myWindow.BeginFrame()) {

        glm::mat4 viewMat;
        glm::mat4 perspectiveMat;

        if (moving)
            myCamera.SetPosition(glm::vec3(cos(angle) * 5, 0, sin(angle) * 5));

        myCamera.LookAt(glm::vec3(0, 0, 0));

        viewMat = myCamera.GetViewMatrix();
        perspectiveMat = myCamera.GetPerspectiveMatrix();

        myShader->Bind();
        myShader->SetUniform("color", glm::vec3(1, 1, 1));
        myShader->SetUniform("compositeMatrix", perspectiveMat * viewMat * myModelMatrix);
        myShader->SetUniform("modelMatrix", myModelMatrix);

        myBuffer->DrawArrays(36);

        if (moving)
            angle += 0.01;
        myWindow.EndFrame();
    }
}