#include "Scene/BaseCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

BaseCamera::BaseCamera() :
    m_Position({ 0.0f, 0.0f, 0.0f }),
    m_Front({ 0.0f, 0.0f, -1.0f }),
    m_Right({ 1.0f, 0.0f, 0.0f }),
    m_Up({ 0.0f, 1.0f, 0.0f }),
    m_MatWorldToView(glm::mat4(1.0f)),
    m_VerticalFov(45.0f) {}

void BaseCamera::Animate(const float& deltaTime) {}

void BaseCamera::SetMoveSpeed(const float& speed) {
    m_MoveSpeed = speed;
}

void BaseCamera::SetRotateSpeed(const float& speed) {
    m_RotateSpeed = speed;
}

void BaseCamera::SetVerticalFov(const float& fovy) {
    m_VerticalFov = fovy;
}

void BaseCamera::BaseLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
    m_Position = position;
    m_Front = glm::normalize(target - position);
    m_Up = glm::normalize(worldUp);
    m_Right = glm::normalize(glm::cross(m_Front, m_Up));
    m_Up = glm::normalize(glm::cross(m_Right, m_Front));

    UpdateWorldToView();
}

void BaseCamera::UpdateWorldToView() {
    //    glm::mat4 rotation = glm::mat4( glm::vec4( m_Right, 0.0f ),
    //                                    glm::vec4( m_Up, 0.0f ),
    //                                    glm::vec4( m_Front, 0.0f ),
    //                                    glm::vec4( 0.0f ) );
    //    glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_Position);
    //    m_MatWorldToView = rotation;
    m_MatWorldToView = glm::lookAt(m_Position, m_Position + m_Front, { 0.0f, 1.0f, 0.0f });
}