#include "Camera.h"

Camera::Camera(int startMode)
{
    mode = startMode;
}

void Camera::setMode(int newMode)
{
    mode = newMode;
}

int Camera::getMode() const
{
    return mode;
}

glm::mat4 Camera::getViewMatrix() const
{
    if (mode == 1)
    {
        // First View
        return glm::lookAt(glm::vec3(0.0f, 2.0f, 12.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f));
    }
    else if (mode == 2)
    {
        // Straight front view
        return glm::lookAt(glm::vec3(0.0f, 0.0f, 12.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 1.5f));
    }
    else if (mode == 3)
    {
        // Top-down view
        return glm::lookAt(glm::vec3(0.0f, 20.0f, 0.01f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, -1.0f));
    }

    // fallback: default straight view
    return glm::lookAt(glm::vec3(0.0f, 2.0f, 6.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));
}
