#include "../Oxide/src/Oxide.h"

int main(int argc, char *argv[]) {
    

    Oxide::Window myWindow({"First application", 720, 500, true});

    Oxide::Ref<Oxide::VertexBuffer> myBuffer = Oxide::VertexBuffer::Create();

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

    while (!myWindow.BeginFrame()) {

        myBuffer->DrawArrays(2);

        myWindow.EndFrame();

    }

}