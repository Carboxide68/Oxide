#pragma once

namespace Oxide {

    class RendererAPI {
    
    public:
        enum class API {
            None = 0,
            OpenGL = 1,
        };

        static API GetAPI();


    private:

        static API s_API; 

    };
}