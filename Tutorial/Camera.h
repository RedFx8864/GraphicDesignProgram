#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera
{
private:
    int mode; // 1,2,3

public:
    Camera(int startMode = 1);

    void setMode(int newMode);
    int getMode() const;

    // returns a view matrix based on current mode
    glm::mat4 getViewMatrix() const;
};

#endif
