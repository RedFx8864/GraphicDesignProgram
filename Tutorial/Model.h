#include "Model.h"
#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const std::string& path) : loaded(false) {
    if (!path.empty()) {
        loadFromFile(path);
    }
}

bool Model::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << " Failed to open model file: " << path << std::endl;
        return false;
    }

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;
    std::vector<unsigned int> vertexIndices, normalIndices, texCoordIndices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        }
        else if (type == "vt") {
            glm::vec2 tex;
            ss >> tex.x >> tex.y;
            tempTexCoords.push_back(tex);
        }
        else if (type == "vn") {
            glm::vec3 normal;
            ss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (type == "f") {
            unsigned int v, t, n;
            char slash;

            for (int i = 0; i < 3; i++) {
                ss >> v >> slash >> t >> slash >> n;
                vertexIndices.push_back(v);
                texCoordIndices.push_back(t);
                normalIndices.push_back(n);
            }
        }
    }

    file.close();

    Mesh mesh;
    for (size_t i = 0; i < vertexIndices.size(); i++) {
        glm::vec3 vertex = tempVertices[vertexIndices[i] - 1];
        glm::vec2 tex = tempTexCoords[texCoordIndices[i] - 1];
        glm::vec3 normal = tempNormals[normalIndices[i] - 1];

        mesh.vertices.push_back(vertex);
        mesh.texCoords.push_back(tex);
        mesh.normals.push_back(normal);
        mesh.indices.push_back(static_cast<unsigned int>(i));
    }

    setupMesh(mesh);
    meshes.push_back(mesh);
    loaded = true;

    std::cout << " Loaded model: " << path << " with " << mesh.vertices.size() << " vertices.\n";
    return true;
}

void Model::setupMesh(Mesh& mesh) {
    glGenVertexArrays(1, &mesh.vao);
    glGenBuffers(1, &mesh.vboVertices);
    glGenBuffers(1, &mesh.vboNormals);
    glGenBuffers(1, &mesh.vboTexCoords);
    glGenBuffers(1, &mesh.ebo);

    glBindVertexArray(mesh.vao);

    // Vertex positions
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboVertices);
    glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(glm::vec3), mesh.vertices.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Normals
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboNormals);
    glBufferData(GL_ARRAY_BUFFER, mesh.normals.size() * sizeof(glm::vec3), mesh.normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Texture coordinates
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vboTexCoords);
    glBufferData(GL_ARRAY_BUFFER, mesh.texCoords.size() * sizeof(glm::vec2), mesh.texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), mesh.indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Model::draw() const {
    for (const auto& mesh : meshes) {
        glBindVertexArray(mesh.vao);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}

void Model::cleanup() {
    for (auto& mesh : meshes) {
        if (mesh.vao) glDeleteVertexArrays(1, &mesh.vao);
        if (mesh.vboVertices) glDeleteBuffers(1, &mesh.vboVertices);
        if (mesh.vboNormals) glDeleteBuffers(1, &mesh.vboNormals);
        if (mesh.vboTexCoords) glDeleteBuffers(1, &mesh.vboTexCoords);
        if (mesh.ebo) glDeleteBuffers(1, &mesh.ebo);
    }
    meshes.clear();
    loaded = false;
}
