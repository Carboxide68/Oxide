#pragma once

#include "Oxide/Core/Base.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace Oxide {

    //TODO: Fix math library

    class PerspectiveCamera {
    public:

        PerspectiveCamera();
        
        const glm::mat4& GetViewMatrix() const;
        const glm::mat4& GetPerspectiveMatrix() const;

        float GetFOV() const;
        const float& GetFOVRadians() const;
        void SetFOV(const float& degrees);
        void SetFOVRadians(const float& radians);

        void SetNear(const float& near);
        void SetFar(const float& far);
        const float& GetNear() const;
        const float& GetFar() const;

        glm::vec3 GetPosition() const;
        void SetPosition(const glm::vec3& position);
        void Move(const glm::vec3& position);

        void LookAt(const glm::vec3& position); //Looks at object without rotating around x-axis
        glm::vec3 GetLookAt() const; //Returns a vector with a length of 1 in the direction which it's looking at

        static Ref<PerspectiveCamera> Create();

    private:

        void UpdatePerspectiveMatrix();

        glm::mat4 m_ViewMatrix;
        glm::mat4 m_PerspectiveMatrix;

        float m_FOV;
        float m_Near;
        float m_Far;

    };

    class OrthographicCamera {  //TODO: Implement Orthographic camera

        OrthographicCamera();

    };

    typedef PerspectiveCamera Camera;
}