#include "Camera.h"

void Camera::ResetPosition()
{
    m_position = glm::vec3(0.0f, 10.0f, 3.0f);
}

void Camera::ResetFront()
{
    m_front = glm::vec3(0.0f, 0.0f, -1.0f);
}

void Camera::ResetUp()
{
    m_up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::ProcessPosition(const Uint8* keystate, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;

    if (keystate[SDL_SCANCODE_W])
        m_position += m_front * velocity;

    if (keystate[SDL_SCANCODE_S])
        m_position -= m_front * velocity;

    if (keystate[SDL_SCANCODE_A]) 
        m_position += glm::normalize(glm::cross(m_front, m_up)) * velocity;

    if (keystate[SDL_SCANCODE_D])
        m_position -= glm::normalize(glm::cross(m_front, m_up)) * velocity;

    if (keystate[SDL_SCANCODE_SPACE])
        m_position += m_up * velocity;

    if (keystate[SDL_SCANCODE_LCTRL])
        m_position -= m_up * velocity;

    if (keystate[SDL_SCANCODE_DOWN])
        ResetPosition();

    if (keystate[SDL_SCANCODE_UP])
        ResetUp();

    if (keystate[SDL_SCANCODE_RIGHT])
        ResetFront();
}

void Camera::ProcessYawAndPitchMovement(const Uint8* keystate, float deltaTime)
{
    const float velocity = m_rotationSpeed * deltaTime;

    if (keystate[SDL_SCANCODE_Q])
        m_yaw += velocity; // Rotate left (look left)

    if (keystate[SDL_SCANCODE_E])
        m_yaw -= velocity; // Rotate right (look right)

    if (keystate[SDL_SCANCODE_W] && keystate[SDL_SCANCODE_LSHIFT])
        m_pitch += velocity; // Look up
            

    if (keystate[SDL_SCANCODE_S] && keystate[SDL_SCANCODE_LSHIFT])
        m_pitch -= velocity; // Look down
            

    if (keystate[SDL_SCANCODE_A] && keystate[SDL_SCANCODE_LSHIFT])
        m_yaw += velocity; // Rotate left
            

    if (keystate[SDL_SCANCODE_D] && keystate[SDL_SCANCODE_LSHIFT])
        m_yaw -= velocity; // Rotate right
            

    // Clamp to prevent gimbal lock
    if (m_pitch > kMaxPitch) m_pitch = kMaxPitch;
    if (m_pitch < -kMaxPitch) m_pitch = -kMaxPitch;

    glm::vec3 front;

    // cos(yaw) * cos(pitch)
    front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    // sin(pitch)
    front.y = sin(glm::radians(m_pitch));

    // sin(yaw) * cos(pitch)
    front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = normalize(front);
}

void Camera::CameraGetViewMatrix(Camera* pCamera, glm::mat4* pMatrix)
{
    *pMatrix = glm::lookAt(pCamera->m_position, pCamera->m_position + pCamera->m_front, pCamera->m_up);
}

void Camera::CameraUpdate(float tickDelta)
{
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    if (keystate == nullptr)
        return;

    if (!keystate[SDL_SCANCODE_LSHIFT]) // holding shift will disable camera movement
    {
        this->ProcessPosition(keystate, tickDelta);
    }

    this->ProcessYawAndPitchMovement(keystate, tickDelta);
}
