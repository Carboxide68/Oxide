#pragma once

#include "Oxide/Core/Base.h"

namespace Oxide {

    //TODO: Fix math library

    class Camera {

        virtual ~Camera() = default;

        //virtual const glm::mat4& GetViewMatrix();

    };

    class PerspectiveCamera : Camera {

        PerspectiveCamera();

        //virtual const glm::mat4& GetViewMatrix();

    };

    class OrthographicCamera : Camera { 

        OrthographicCamera();

        //virtual const glm::mat4& GetViewMatrix();

    };

}