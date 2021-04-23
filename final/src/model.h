// Name: model.h
// Author: Cameron Winningham
// Date: 3/27/2021
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Model and mesh class

#ifndef MODEL_H
#define MODEL_H

#include "material.h"
#include "geometry.h"

// Stores the GL data relative to a given mesh:
class Mesh {
public:
    Mesh();

    void destroy();

    GLuint vao; // handle for the vertex array object
    GLuint vbo; // handles for the vertex buffer object for vertices
    GLuint indexBuffer; // index buffer for indexed rendering data
    GLuint uvBuffer; // buffer for UV data
    GLuint normalBuffer; // buffer for normal data
    GLuint tangentBuffer; // buffer for tangent data
    GLuint bitangentBuffer; // buffer for bitangent data
    GLuint nIndices; // number of indices of the mesh
    GLuint nVertices; // number of verties of the mesh
    GLuint totalUVs; // number of UVs of the mesh
    GLuint totalNormals; // number of normals of the mesh
    Material material; // the mesh's material
    bool textured; // does the mesh have a texture?
};

// Model base class:
class Model {
public:
    Model();

    virtual bool init() { return false; };

    virtual void destroy();

    virtual void render() = 0;

    bool loadTexture(const char *filename);

    bool loadNormalMap(const char *filename); // this is currently only supported for a cube

    void setPosition(float x, float y, float z);

    void setRotation(float x, float y, float z);

    void setScale(float x, float y, float z);

    glm::vec3 getPosition();

    glm::vec3 getRotation();

    glm::vec3 getScale();

    bool hasMaterial();

    Material *getMaterial();

protected:
    Mesh mesh;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};

#endif //MODEL_H
