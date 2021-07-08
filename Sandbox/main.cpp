#include "../Oxide/src/Oxide.h"
#include "../Oxide/src/Oxide/Core/Log.h"

int main(int argc, char *argv[]) {

    Oxide::Log::Init();
    Oxide::Log::GetCoreLogger()->set_level(spdlog::level::info);
    Oxide::Log::GetClientLogger()->set_level(spdlog::level::info);

    Oxide::Window myWindow({"First application", 720, 500, true});
    myWindow.renderer->SetClearColor(1, 1, 1, 1);
    myWindow.renderer->ChangeState(Oxide::CRenderer::RenderSettings::FACE_CULLING, false);
    glEnable(GL_DEPTH_TEST);

    Oxide::Ref<Oxide::VertexBuffer> myBuffer = Oxide::VertexBuffer::Create();

    Oxide::Camera myCamera;
    myCamera.SetFOV(70.0f);
    myCamera.SetAspect((float)myWindow.GetWidth()/(float)myWindow.GetHeight());
    myWindow.renderer->SetViewport(720, 500);
    float angle = 0;
    myCamera.SetPosition(glm::vec3(cos(angle) * 5, 0, sin(angle) * 5));

    Oxide::Ref<Oxide::Shader> myShader = Oxide::Shader::Create("Sandbox/shader/myShader.OS");

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

    glm::mat4 myModelMatrix(1);
    myModelMatrix[0][0] = 1;
    myModelMatrix[1][1] = 1;
    myModelMatrix[2][2] = 1;

    myBuffer->BufferData(sizeof(data), (void*)data);
    myBuffer->AddElementToBufferLayout(Oxide::OxideType::Float, 3);
    myBuffer->AddElementToBufferLayout(Oxide::OxideType::Float, 2);

    // Oxide::Ref<Oxide::IndexBuffer> myIndexBuffer = Oxide::IndexBuffer::Create();
    // myIndexBuffer->BufferData(sizeof(indexData), (void*)indexData);
    // myBuffer->AssociateIndexBuffer(myIndexBuffer);


    bool moving = true;
    myWindow.eventhandler->OnMouseButtonEvent([&] (Oxide::OxideEvent button, Oxide::OxideEvent action) -> void {
        if (action == Oxide::OxideEvent::Pressed && button == Oxide::OxideEvent::MOUSE_BUTTON_1) {
            moving = !moving;
        }
    });

    bool wireFrame = false;
    myWindow.eventhandler->OnKeyEvent([&] (Oxide::OxideEvent key, Oxide::OxideEvent action) -> void {

        if (key == Oxide::OxideEvent::KEY_LEFT_ALT && action == Oxide::OxideEvent::Pressed) {
            if (!wireFrame) {
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireFrame = !wireFrame;
            } else {
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireFrame = !wireFrame;
            }
        }
    });

    myWindow.eventhandler->OnKeyEvent ([&] (Oxide::OxideEvent key, Oxide::OxideEvent action) -> void {
        if (key == Oxide::OxideEvent::KEY_ESCAPE && action == Oxide::OxideEvent::Released) {
            glfwSetWindowShouldClose(myWindow.GetGLFWWindow(), true);
        }
    });
    Oxide::Ref<Oxide::Texture2D> myTexture = Oxide::Texture2D::Create("Sandbox/rsrc/texture1.jpeg");
    myTexture->SetName("texture1");

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
        myTexture->Bind(0);
        myShader->SetUniform(myTexture->GetName(), 0);

        myBuffer->DrawArrays(36);

        if (moving)
            angle += 0.01;
        myWindow.EndFrame();
    }
}