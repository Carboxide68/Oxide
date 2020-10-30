#include "../Oxide/src/Oxide.h"

int main(int argc, char *argv[]) {
    

    Oxide::Window myWindow({"First application", 720, 500, true});

    while (myWindow.BeginFrame()) {

        myWindow.EndFrame();

    }

}