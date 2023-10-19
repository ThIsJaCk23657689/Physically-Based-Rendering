#include "Scene/FPSCamera.hpp"
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

    bool cameraDirty = false;
    glm::mat4 cameraRotation = glm::mat4(1.0f);

    // Handle mouse rotation first
    // this will affect the movement vectors in the world matrix
    if (mouseButtonState[MouseButtons::Left] && (mouseMove.x || mouseMove.y)) {
        float yaw = m_RotateSpeed * mouseMove.x;
        float pitch = m_RotateSpeed * mouseMove.y;

        cameraRotation = glm::rotate(cameraRotation, glm::radians(-yaw), glm::vec3(0.0f, 1.0f, 0.0f));
        cameraRotation = glm::rotate(cameraRotation, glm::radians(pitch), m_Right);

        cameraDirty = true;
    }

    // Handle translation
    float moveStep = deltaTime * m_MoveSpeed;
    glm::vec3 cameraMoveVector = glm::vec3(0.0f);

    if (keyboardState[KeyboardControls::SpeedUp]) {
        moveStep *= 3.0f;
    }

    if (keyboardState[KeyboardControls::SlowDown]) {
        moveStep *= 0.1f;
    }

    if (keyboardState[KeyboardControls::MoveForward]) {
        cameraDirty = true;
        cameraMoveVector += m_Front * moveStep;
    }

    if (keyboardState[KeyboardControls::MoveBackward]) {
        cameraDirty = true;
        cameraMoveVector += -m_Front * moveStep;
    }

    if (keyboardState[KeyboardControls::MoveLeft]) {
        cameraDirty = true;
        cameraMoveVector += -m_Right * moveStep;
    }

    if (keyboardState[KeyboardControls::MoveRight]) {
        cameraDirty = true;
        cameraMoveVector += m_Right * moveStep;
    }

    if (cameraDirty) {
        m_Position += cameraMoveVector;
        m_Front = glm::normalize(cameraRotation * glm::vec4(m_Front, 1.0f));
        m_Up = glm::normalize(cameraRotation * glm::vec4(m_Up, 1.0f));
        m_Right = glm::normalize(glm::cross(m_Front, m_Up));

        UpdateWorldToView();
    }
}