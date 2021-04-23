// Name: model.cpp
// Author: Cameron Winningham
// Date: 3/27/2021
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Model and mesh class

#include "model.h"
#include "shader.h"
#include "opengl.h" // texture loading

Mesh::Mesh() {
    // Initialize everything:
    vao = 0;
    vbo = 0;
    indexBuffer = 0;
    uvBuffer = 0;
    normalBuffer = 0;
    tangentBuffer = 0;
    bitangentBuffer = 0;
    nIndices = 0;
    nVertices = 0;
    totalUVs = 0;
    totalNormals = 0;
    textured = false;
}

void Mesh::destroy() {
    // Destroy textures:
    if (textured) {
        material.diffuse.destroy();
    }
    if (material.useNormalMap) {
        material.normal.destroy();
    }

    glDeleteVertexArrays(1, &vao); // delete VAO
    glDeleteBuffers(1, &vbo); // delete VBO
    glDeleteBuffers(1, &indexBuffer); // delete index buffer
    glDeleteBuffers(1, &uvBuffer); // delete UV buffer
    glDeleteBuffers(1, &normalBuffer); // delete normal buffer
    glDeleteBuffers(1, &tangentBuffer); // delete tangent buffer
    glDeleteBuffers(1, &bitangentBuffer); // delete bitangent buffer
}

Model::Model() {
    // Default position, rotation, and scale:
    mesh = {};
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.0f, 1.0f, 1.0f);
}

bool Model::loadTexture(const char *filename) {
    // Load the texture:
    if (!mesh.material.diffuse.load(filename)) {
        printf("ERROR: Failed to load %s\n", filename);
        return false;
    } else {
        mesh.textured = true;
        return true;
    }
}

// Currently only supported for a cube:
bool Model::loadNormalMap(const char *filename) {
    // Load the normal map:
    if (!mesh.material.normal.load(filename, 1, GL_TEXTURE_2D, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR)) {
        printf("ERROR: Failed to load %s\n", filename);
        return false;
    } else {
        mesh.material.useNormalMap = true;
        return true;
    }
}

void Model::destroy() {
    mesh.destroy();
}

void Model::setPosition(float x, float y, float z) {
    position = glm::vec3(x, y, z);
}

void Model::setRotation(float x, float y, float z) {
    rotation = glm::vec3(RADIAN(x), RADIAN(y), RADIAN(z));
}

void Model::setScale(float x, float y, float z) {
    scale = glm::vec3(x, y, z);
}

glm::vec3 Model::getPosition() {
    return position;
}

glm::vec3 Model::getRotation() {
    return rotation;
}

glm::vec3 Model::getScale() {
    return scale;
}

bool Model::hasMaterial() {
    return mesh.textured;
}

Material *Model::getMaterial() {
    return &mesh.material;
}
