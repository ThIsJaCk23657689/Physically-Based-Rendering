#ifndef BASECAMERA_HPP
#define BASECAMERA_HPP

#include <string>
#include <SDL.h>
#include <glm/glm.hpp>

class BaseCamera {
public:
    virtual void OnKeyboardEvent(const SDL_KeyboardEvent& event) {}
    virtual void OnMouseButtonEvent(const SDL_MouseButtonEvent& event) {}
    virtual void OnMouseMotionEvent(const SDL_MouseMotionEvent& event) {}
    virtual void OnMouseWheelEvent(const SDL_MouseWheelEvent& event) {}
    virtual void Animate(const float& deltaTime);

    void SetMoveSpeed(const float& speed);
    void SetRotateSpeed(const float& speed);
    void SetVerticalFov(const float& fovy);
    const float& GetVerticalFov() const { return m_VerticalFov; }

    virtual const glm::mat4& GetWorldToViewMatrix() const { return m_MatWorldToView; }

public:
    std::string m_Name;
    float m_VerticalFov;
    float m_Pitch;                 // In deg
    float m_Yaw;                   // In deg
    glm::vec3 m_Position;          // In world space
    glm::vec3 m_Velocity;
    glm::vec3 m_Acceleration;
    glm::vec3 m_Right;             // normalized
    glm::vec3 m_Front;             // normalized
    glm::vec3 m_Up;                // normalized

protected:
    BaseCamera();

    void BaseLookAt(glm::vec3 position, glm::vec3 target, glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f });
    void UpdateWorldToView();

    glm::mat4 m_MatWorldToView;

    float m_MoveSpeed = 1.0f;      // movement speed int units/second
    float m_RotateSpeed = 0.1f;    // mouse sensitivity in radians/pixel
};

#endif
