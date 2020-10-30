#include "../Oxide/src/Oxide.h"

int main(int argc, char *argv[]) {
    

    Oxide::Window myWindow({"First application", 720, 500, true});

    Oxide::Ref<Oxide::VertexBuffer> myBuffer = Oxide::VertexBuffer::Create();

    Oxide::Ref<Oxide::Shader> myShader = Oxide::Shader::Create("shader/myShader.OxideShader");

    float data[] = {
        0.5f, 0.5f,
        0.5f, -0.5f,
        -0.5f, -0.5f,

        -0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f
    };

    myBuffer->BufferData(sizeof(data), (void*)data);
    myBuffer->AddElementToBufferLayout(Oxide::OxideType::Float, 2); 

    myShader->SetUniform("color", glm::vec3(0, 1, 0));

    while (!myWindow.BeginFrame()) {

        myShader->Bind();
        myBuffer->DrawArrays(2);

        myWindow.EndFrame();

    }

}