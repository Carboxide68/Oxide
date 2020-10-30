#include "Oxide/Renderer/Camera.h"
#include "glm/trigonometric.hpp"

namespace Oxide {

    PerspectiveCamera::PerspectiveCamera() {

        m_ViewMatrix = glm::mat4(1);

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

    const glm::mat4& PerspectiveCamera::GetViewMatrix() const {
        return m_ViewMatrix;
    }

    const glm::mat4& PerspectiveCamera::GetPerspectiveMatrix() const {
        return m_PerspectiveMatrix;
    }
    
    glm::vec3 PerspectiveCamera::GetPosition() const {

        glm::vec3 position;
        position.x = m_ViewMatrix[3][0];
        position.y = m_ViewMatrix[3][1];
        position.z = m_ViewMatrix[3][2];

        return position;
    }

    void PerspectiveCamera::SetPosition(const glm::vec3& position) {
        m_ViewMatrix[3][0] = position.x;
        m_ViewMatrix[3][1] = position.y;
        m_ViewMatrix[3][2] = position.z;
    }

    void PerspectiveCamera::Move(const glm::vec3& position) {
        m_ViewMatrix[3][0] += position.x;
        m_ViewMatrix[3][1] += position.y;
        m_ViewMatrix[3][2] += position.z;
    }
                                                                //TODO: Make sure that it doesn't become bad if position is too close to x = 1
    void PerspectiveCamera::LookAt(const glm::vec3& position) { //FIXME: Test if this actually works, I have no idea
        glm::vec3 UP = glm::vec3(0, 1, 0);

        glm::vec3 z = glm::normalize(position);
        glm::vec3 x = glm::cross(z, UP); //Will always be normalized since we have 2 vectors with length 1
        glm::vec3 y = glm::cross(x, z);
        z *= -1;

        glm::vec3 p = -GetPosition(); //Position, need it to be n.gative in the matrix for things to be translated correctly

        m_ViewMatrix = glm::mat4(
            x.x, x.y, x.z, 0,
            y.x, y.y, y.z, 0,
            z.x, z.y, z.z, 0,
            p.x, p.y, p.z, 1
        );
    }
    
    glm::vec3 PerspectiveCamera::GetLookAt() const {
        glm::vec3 position;

        position.x = m_ViewMatrix[2][0];
        position.y = m_ViewMatrix[2][1];
        position.z = m_ViewMatrix[2][2];

        return -position;
    }

    static Ref<PerspectiveCamera> Create() {

        return CreateRef<PerspectiveCamera>();

    }

    void PerspectiveCamera::UpdatePerspectiveMatrix() { //TODO: Make sure that this is oriented correctly

        float S = 1/tan(m_FOV/2);

        float tmp1 = m_Far/(m_Far - m_Near);
        float tmp2 = (m_Far * m_Near)/(m_Far - m_Near);

        m_PerspectiveMatrix = glm::mat4(
            S,  0,  0   ,   0   ,
            0,  S,  0   ,   0   ,
            0,  0,  tmp1,   tmp2,
            0,  0, -1   ,   0   

        );

    }

}