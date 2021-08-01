#include "Oxide/Renderer/Camera.h"
#include "glm/trigonometric.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Oxide/Renderer/CRenderer.h"

namespace Oxide {

    PerspectiveCamera::PerspectiveCamera() {

        type = CameraType::Orthographic;
        m_ViewMatrix = glm::mat4(1);
        m_Far = 100.0f;
        m_Near = 0.1f;
        m_FOV = 90.0f;
        m_Aspect = (float)(16.0/9.0);
        m_LookingDir = {1, 0, 0};
        UpdatePerspectiveMatrix();

    }

    float PerspectiveCamera::GetFOV() const {
        return glm::degrees(m_FOV);
    }

    const float& PerspectiveCamera::GetFOVRadians() const {
        return m_FOV;
    }

    void PerspectiveCamera::SetFOV(const float& degrees) {
        SetFOVRadians(glm::radians(degrees));
    }

    void PerspectiveCamera::SetFOVRadians(const float& radians) {
        m_FOV = radians;
        UpdatePerspectiveMatrix();
    }

    void PerspectiveCamera::SetAspect(const float& aspect) {
        m_Aspect = aspect;
        UpdatePerspectiveMatrix();
    }

    const float& PerspectiveCamera::GetAspect() const {
        return m_Aspect;
    }

    void PerspectiveCamera::SetNear(const float& near) {
        m_Near = near;
        UpdatePerspectiveMatrix();
    }

    void PerspectiveCamera::SetFar(const float& far) {
        m_Far = far;
        UpdatePerspectiveMatrix();
    }

    const float& PerspectiveCamera::GetNear() const {
        return m_Near;
    }

    const float& PerspectiveCamera::GetFar() const {
        return m_Far;
    }

    const glm::mat4& PerspectiveCamera::GetViewMatrix() {
        if (!m_UpdateViewMatrix) return m_ViewMatrix;
        glm::vec3 UP = glm::vec3(0, 1, 0);

        glm::vec3 z = m_LookingDir;
        glm::vec3 x = glm::cross(z, UP); //Will always be normalized since we have 2 vectors with length 1
        glm::vec3 y = glm::cross(x, z);

        glm::vec3 p = m_Position;
        p = glm::vec3(-glm::dot(x, p), -glm::dot(y, p), -glm::dot(z, p));

        m_ViewMatrix = glm::mat4(
            x.x, y.x, z.x, 0,
            x.y, y.y, z.y, 0,
            x.z, y.z, z.z, 0,
            p.x, p.y, p.z, 1
        );
        return m_ViewMatrix;
    }

    const glm::mat4& PerspectiveCamera::GetPerspectiveMatrix() const {
        return m_PerspectiveMatrix;
    }
    
    const glm::vec3& PerspectiveCamera::GetPosition() const {
        return m_Position;
    }
    void PerspectiveCamera::SetPosition(const glm::vec3& position) {
        m_Position = position;
        m_UpdateViewMatrix = true;
    }

    void PerspectiveCamera::Move(const glm::vec3& position) {
        m_Position += position;
        m_UpdateViewMatrix = true;
    }
                                                                //TODO: Make sure that it doesn't become bad if position is too close to x = 1
    void PerspectiveCamera::LookAt(const glm::vec3& position, bool relative) {
        m_LookingDir = glm::normalize((relative) ? position : position - m_Position);
        m_UpdateViewMatrix = true;
    }
    
    const glm::vec3& PerspectiveCamera::GetLookAt() const {
        return m_LookingDir;
    }

    void PerspectiveCamera::UpdatePerspectiveMatrix() { //TODO: Make sure that this is oriented correctly

        float S = 1.0/tan(m_FOV/2.0);
        float Sx = S / m_Aspect;
        float Sy = S;

        float tmp1 = m_Far/(m_Far - m_Near);
        float tmp2 = -(m_Far * m_Near)/(m_Far - m_Near);

        m_PerspectiveMatrix = glm::mat4(
            Sx,   0,    0   ,   0,
            0 ,  Sy,    0   ,   0,
            0 ,   0,    tmp1,   1,
            0 ,   0,    tmp2,   0   

        );
    }

    Ref<PerspectiveCamera> PerspectiveCamera::Create() {
        return Ref<PerspectiveCamera>(new PerspectiveCamera());
    }

    OrthographicCamera::OrthographicCamera() {
        type = CameraType::Orthographic;
    }
    
    const glm::mat4& OrthographicCamera::GetViewMatrix() {return m_ViewMatrix;}
    const glm::mat4& OrthographicCamera::GetPerspectiveMatrix() const {return m_PerspectiveMatrix;}

    void OrthographicCamera::SetNear(const float& near) {return;}
    void OrthographicCamera::SetFar(const float& far) {return;}
    const float& OrthographicCamera::GetNear() const {return 0.0f;}
    const float& OrthographicCamera::GetFar() const {return 0.0f;}

    void OrthographicCamera::SetAspect(const float& aspect) {return;}
    const float& OrthographicCamera::GetAspect() const {return 0.0f;}

    const glm::vec3& OrthographicCamera::GetPosition() const {return {0, 0, 0};}
    void OrthographicCamera::SetPosition(const glm::vec3& position) {return;}
    void OrthographicCamera::Move(const glm::vec3& position) {return;}

    void OrthographicCamera::LookAt(const glm::vec3& position, bool relative) {return;} //Looks at object without rotating around x-axis
    const glm::vec3& OrthographicCamera::GetLookAt() const {return {0, 0, 0};}

}