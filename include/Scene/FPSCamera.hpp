#ifndef FPSCAMERA_HPP
#define FPSCAMERA_HPP

#include <SDL3/SDL.h>

#include <array>
#include <glm/glm.hpp>
#include <map>

#include "BaseCamera.hpp"

struct CameraPreset
{
    std::string name;
    glm::vec3 position = { 0.0, 0.0, 0.0 };
    glm::vec3 target = { 0.0, 0.0, -1.0 };
    glm::vec3 up = { 0.0f, 1.0f, 0.0f };
    float fovY = 45.0f;
};

class FPSCamera : public BaseCamera
{
public:
    void OnKeyboardEvent( const SDL_KeyboardEvent& event ) override;
    void OnMouseButtonEvent( const SDL_MouseButtonEvent& event ) override;
    void OnMouseMotionEvent( const SDL_MouseMotionEvent& event ) override;
    void OnMouseWheelEvent( const SDL_MouseWheelEvent& event ) override;
    void Animate( SDL_Window* window, const float deltaTime ) override;

    void SwitchCameraCursorMode();
    void LookAt( glm::vec3 position, glm::vec3 target, glm::vec3 worldUp = { 0.0f, 1.0f, 0.0f } );

private:
    bool m_CameraCursorMode = false;
    bool m_MouseInitialized = false;
    glm::vec2 m_MouseRelPos;

    typedef enum
    {
        MoveForward,
        MoveLeft,
        MoveRight,
        MoveBackward,

        MoveUp,
        MoveDown,

        YawRight,
        YawLeft,
        PitchUp,
        PitchDown,
        RollLeft,
        RollRight,

        SpeedUp,
        SlowDown,

        KeyboardControlCount
    } KeyboardControls;

    typedef enum
    {
        Left,
        Middle,
        Right,

        MouseButtonCount,
        MouseButtonFirst = Left,
    } MouseButtons;

    const std::map< int, int > keyboardMap = {
        { SDL_SCANCODE_W, KeyboardControls::MoveForward },
        { SDL_SCANCODE_A, KeyboardControls::MoveLeft },
        { SDL_SCANCODE_S, KeyboardControls::MoveBackward },
        { SDL_SCANCODE_D, KeyboardControls::MoveRight },

        { SDL_SCANCODE_SPACE, KeyboardControls::MoveUp },
        { SDL_SCANCODE_LCTRL, KeyboardControls::MoveDown },
    };

    const std::map< int, int > mouseButtonMap = { { SDL_BUTTON_LEFT, MouseButtons::Left },
                                                  { SDL_BUTTON_MIDDLE, MouseButtons::Middle },
                                                  { SDL_BUTTON_RIGHT, MouseButtons::Right } };

    std::array< bool, KeyboardControls::KeyboardControlCount > keyboardState = { false };
    std::array< bool, MouseButtons::MouseButtonCount > mouseButtonState = { false };
};

#endif
