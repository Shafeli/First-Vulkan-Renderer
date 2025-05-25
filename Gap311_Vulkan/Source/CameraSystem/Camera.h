#pragma once
#include <glm/geometric.hpp>
#include <SDL2/SDL.h>
#include <glm/vec3.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Camera
{
public:
    static constexpr float kYaw = -90.0f;
    static constexpr float kPitch = 0.0f;
    static constexpr float kMovementSpeed = 12.5f;
    static constexpr float kRotationSpeed = 19.5f;

    static constexpr float kMaxPitch = 89.0f; // Clamp values to prevent gimbal lock 
    static constexpr float kMinPitch = -89.0f;

    float m_yaw = kYaw;
    float m_pitch = kPitch;
    float m_movementSpeed = kMovementSpeed;
    float m_rotationSpeed = kMovementSpeed;

    glm::vec3 m_position = glm::vec3(0.394f, 7.150f, 18.903f);
    glm::vec3 m_front = glm::vec3(-0.014f, -0.223f, -0.957f);
    glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);

    void ResetPosition();

    void ResetFront();

    void ResetUp();

    void ProcessPosition(const Uint8* keystate, float deltaTime);

    void ProcessYawAndPitchMovement(const Uint8* keystate, float deltaTime);

    void CameraGetViewMatrix(Camera* pCamera, glm::mat4* pMatrix);
    void CameraUpdate(float tickDelta);
};

