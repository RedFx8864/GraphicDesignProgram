#include "Orbit.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#define M_PI 3.14159265358979323846



// Constructor
Orbit::Orbit(float radius, float speed, float scale)
    : radius(radius), orbitSpeed(speed), objectScale(scale), orbitAngle(0.0f), vao(0), vbo(0)
{
}
//Init
void Orbit::init()
{
    generateOrbitVertices();
}

// Update the angle based on delta time
void Orbit::update(float deltaTime)
{
    orbitAngle += orbitSpeed * deltaTime;
    if (orbitAngle >= 360.0f) orbitAngle -= 360.0f;
}

// Compute the model matrix for the orbiting object
glm::mat4 Orbit::getModelMatrix(const glm::mat4& centerModelMatrix) const
{
    float angleRad = glm::radians(orbitAngle);

    // Orbit offset in XZ plane
    glm::vec3 offset(radius * cos(angleRad), 0.0f, radius * sin(angleRad));

    // Center position
    glm::vec3 centerPos(centerModelMatrix[3]); // translation component

    // Compute final model matrix
    glm::mat4 model = glm::translate(glm::mat4(1.0f), centerPos + offset);

    // Optional: rotate around its own axis
    model = glm::rotate(model, angleRad, glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply scaling
    model = glm::scale(model, glm::vec3(objectScale));

    return model;
}

// Generate orbit circle vertices
void Orbit::generateOrbitVertices()
{
    orbitVertices.clear();
    const int slices = 100; // resolution of the orbit ring
    for (int i = 0; i <= slices; i++)
    {
        float theta = (2.0f * M_PI * i) / slices;
        orbitVertices.emplace_back(radius * cos(theta), 0.0f, radius * sin(theta));
    }

    // Generate VAO/VBO
    if (vao == 0) glGenVertexArrays(1, &vao);
    if (vbo == 0) glGenBuffers(1, &vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, orbitVertices.size() * sizeof(glm::vec3),
        orbitVertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0); // position only
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}

// Draw the orbit path
void Orbit::drawOrbitPath(const glm::mat4& view, const glm::mat4& projection, const glm::mat4& centerModelMatrix, ShaderProgram& shader) const
{
    shader.use();
    shader.setUniform("uColor", glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(centerModelMatrix[3])); // orbit around center
    glm::mat4 MVP = projection * view * model;
    shader.setUniform("uMVP", MVP);

    glBindVertexArray(vao);
    glDrawArrays(GL_LINE_LOOP, 0, orbitVertices.size());
    glBindVertexArray(0);
}

