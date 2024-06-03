#include "Scene/FPSCamera.hpp"
#include "Utility/Math.hpp"
#include <glm/gtc/matrix_transform.hpp>

void FPSCamera::LookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp) {
    // Make the base method public.
    BaseLookAt(position, target, worldUp);
}

void FPSCamera::OnKeyboardEvent(const SDL_KeyboardEvent& event) {
    const auto scancode = event.keysym.scancode;
    if (keyboardMap.find(scancode) == keyboardMap.end()) {
        return;
    }

    auto cameraKey = keyboardMap.at(scancode);
    if (event.state == SDL_PRESSED || event.repeat != 0) {
        keyboardState[cameraKey] = true;
    } else {
        keyboardState[cameraKey] = false;
    }
}

void FPSCamera::OnMouseButtonEvent(const SDL_MouseButtonEvent& event) {
    const auto button = event.button;
    if (mouseButtonMap.find(button) == mouseButtonMap.end()) {
        return;
    }

    auto cameraButton = mouseButtonMap.at(button);
    if (event.state == SDL_PRESSED) {
        mouseButtonState[cameraButton] = true;
    } else {
        mouseButtonState[cameraButton] = false;
    }
}

void FPSCamera::OnMouseMotionEvent(const SDL_MouseMotionEvent& event) {
    m_MousePos = { event.x, event.y };
}

void FPSCamera::OnMouseWheelEvent(const SDL_MouseWheelEvent& event) {}

void FPSCamera::Animate(const float& deltaTime) {
    // track mouse delta
    glm::vec2 mouseMove = m_MousePos - m_MousePosPrev;
    m_MousePosPrev = m_MousePos;

    // Handle mouse rotation first
    bool rotateDirty = false;
    if (mouseButtonState[MouseButtons::Left] && (!Math::IsZero(mouseMove.x) || !Math::IsZero(mouseMove.y))) {
        // update yaw and pitch
        m_Yaw += m_RotateSpeed * mouseMove.x;
        m_Pitch += m_RotateSpeed * -mouseMove.y;

        // constraint
        if (m_Yaw >= 360.0f || m_Yaw <= -360.0f) {
            m_Yaw = 0.0f;
        }
        if (m_Pitch > 89.0f || m_Pitch < -89.0f) {
            m_Pitch = 89.0f;
        }

        rotateDirty = true;
    }

    if (rotateDirty) {
        // currently does not support z-axis rotation
        glm::vec3 WorldUp = { 0.0f, 1.0f, 0.0f };

        // Update rotate matrix
        auto cameraRotation = glm::mat4(1.0f);
        cameraRotation = glm::rotate(cameraRotation, glm::radians(-m_Yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        cameraRotation = glm::rotate(cameraRotation, glm::radians(m_Pitch), glm::vec3(1.0f, 0.0f, 0.0f));

        // Camera always faces neg-z
        glm::vec4 temp_front = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
        temp_front = cameraRotation * temp_front;

        m_Front = glm::normalize(glm::vec3(temp_front));
        m_Right = glm::normalize(glm::cross(m_Front, WorldUp));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));
    }



    // Handle translation
    if (keyboardState[KeyboardControls::SpeedUp]) {
        m_MoveSpeed *= 3.0f;
    }

    if (keyboardState[KeyboardControls::SlowDown]) {
        m_MoveSpeed *= 0.1f;
    }

    if (keyboardState[KeyboardControls::MoveForward]) {
        m_Acceleration += m_Front * m_MoveSpeed;
    }

    if (keyboardState[KeyboardControls::MoveBackward]) {
        m_Acceleration += -m_Front * m_MoveSpeed;
    }

    if (keyboardState[KeyboardControls::MoveLeft]) {
        m_Acceleration += -m_Right * m_MoveSpeed;
    }

    if (keyboardState[KeyboardControls::MoveRight]) {
        m_Acceleration += m_Right * m_MoveSpeed;
    }

    bool translateDirty = false;
    if (!Math::IsZero(glm::length(m_Acceleration)) || !Math::IsZero(glm::length(m_Velocity))) {
        m_Velocity += m_Acceleration;
        m_Acceleration = glm::vec3(0.0f);

        m_Position += m_Velocity * deltaTime;
        m_Velocity *= 0.90f;
        translateDirty = true;
    }

    if ( rotateDirty || translateDirty ) {
        UpdateWorldToView();
    }
}