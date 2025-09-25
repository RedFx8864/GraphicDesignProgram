#pragma once
#include <glm/glm.hpp>
#include <vector>
#include<GLEW/glew.h>
#include "shaderProgram.h"

extern ShaderProgram gOrbitShader;

class Orbit
{
public:

    void init();
    // Constructor
    Orbit(float radius = 2.0f, float speed = 30.0f, float scale = 1.0f);

    // Update the orbit angle (call every frame)
    void update(float deltaTime);

    // Get the model matrix for the orbiting object
    glm::mat4 getModelMatrix(const glm::mat4& centerModelMatrix) const;

    // Draw the orbit path (ring)
    // Draw the orbit path (ring) around a center object
    void drawOrbitPath(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& centerModelMatrix, ShaderProgram& shader) const;


    // Setters
    void setRadius(float r) { radius = r; }
    void setSpeed(float s) { orbitSpeed = s; }
    void setScale(float s) { objectScale = s; }

private:
    float radius;         // orbit radius
    float orbitSpeed;     // degrees per second
    float orbitAngle;     // current angle in degrees
    float objectScale;    // scale of orbiting object

    GLuint vao, vbo;
    std::vector<glm::vec3> orbitVertices;

    void generateOrbitVertices();
};
