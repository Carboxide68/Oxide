#pragma once

#include "Oxide/Core/Base.h"
#include "glm/mat4x4.hpp"
#include "glm/vec3.hpp"

namespace Oxide {

    //TODO: Fix math library

    enum CameraType {
        Orthographic,
        Perspective
    };

    class Camera {
    public:

        virtual ~Camera() = default;

        virtual const glm::mat4& GetViewMatrix() = 0;
        virtual const glm::mat4& GetPerspectiveMatrix() const = 0;

        virtual void SetNear(const float& near) = 0;
        virtual void SetFar(const float& far) = 0;
        virtual const float& GetNear() const = 0;
        virtual const float& GetFar() const = 0;

        virtual void SetAspect(const float& aspect) = 0;
        virtual const float& GetAspect() const = 0;

        virtual const glm::vec3& GetPosition() const = 0;
        virtual void SetPosition(const glm::vec3& position) = 0;
        virtual void Move(const glm::vec3& position) = 0;

        virtual void LookAt(const glm::vec3& position, bool relative = false) = 0; //Looks at object without rotating around x-axis
        virtual const glm::vec3& GetLookAt() const = 0; //Returns a vector with a length of 1 in the direction which it's looking at

        CameraType type;

    };

    class PerspectiveCamera : public Camera {
    public:
        
        const glm::mat4& GetViewMatrix() override;
        const glm::mat4& GetPerspectiveMatrix() const override;

        float GetFOV() const;
        const float& GetFOVRadians() const;
        void SetFOV(const float& degrees);
        void SetFOVRadians(const float& radians);

        void SetNear(const float& near) override;
        void SetFar(const float& far) override;
        const float& GetNear() const override;
        const float& GetFar() const override;

        void SetAspect(const float& aspect) override;
        const float& GetAspect() const override;

        const glm::vec3& GetPosition() const override;
        void SetPosition(const glm::vec3& position) override;
        void Move(const glm::vec3& position) override;

        void LookAt(const glm::vec3& position, bool relative = false) override; //Looks at object without rotating around x-axis
        const glm::vec3& GetLookAt() const override; //Returns a vector with a length of 1 in the direction which it's looking at

        static Ref<PerspectiveCamera> Create();

    private:

        PerspectiveCamera();

        void UpdatePerspectiveMatrix();

        bool m_UpdateViewMatrix = true;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_PerspectiveMatrix;

        glm::vec3 m_Position;
        glm::vec3 m_LookingDir;

        float m_FOV;
        float m_Aspect;
        float m_Near;
        float m_Far;

    };

    class OrthographicCamera : public Camera {  //TODO: Implement Orthographic camera
    public:
        OrthographicCamera();

        const glm::mat4& GetViewMatrix() override;
        const glm::mat4& GetPerspectiveMatrix() const override;

        void SetNear(const float& near) override;
        void SetFar(const float& far) override;
        const float& GetNear() const override;
        const float& GetFar() const override;

        void SetAspect(const float& aspect) override;
        const float& GetAspect() const override;

        const glm::vec3& GetPosition() const override;
        void SetPosition(const glm::vec3& position) override;
        void Move(const glm::vec3& position) override;

        void LookAt(const glm::vec3& position, bool relative = false) override; //Looks at object without rotating around x-axis
        const glm::vec3& GetLookAt() const override;

        static Ref<OrthographicCamera> Create() {return CreateRef<OrthographicCamera>();}

    private:

        glm::mat4 m_ViewMatrix;
        glm::mat4 m_PerspectiveMatrix;

    };
}