// Name: plane.cpp
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Plane primitive

#include "plane.h"

Plane::Plane(float width, float length) {
    this->width = width;
    this->length = length;
}

bool Plane::init(const char *filename) {
    if (!loadTexture(filename)) {
        return false;
    }

    // Vertex data
    GLfloat verts[] = {
            -width, 0, -length,
            width, 0, length,
            width, 0, -length,

            -width, 0, -length,
            -width, 0, length,
            width, 0, length,
    };

    GLfloat uvs[] = {
            0.0f, 0.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 1.0f,
    };

    // Calculate normals:
    mesh.totalNormals = 6; // one normal per vertex
    glm::vec3 normals[6];
    for (int i = 0; i < mesh.totalNormals; ++i) {
        normals[i] = {0.0f, 1.0f, 0.0f}; // all normals facing straight up
    }

    mesh.nVertices = sizeof(verts);

    glGenVertexArrays(1, &mesh.vao); // create VAO
    glBindVertexArray(mesh.vao); // bind VAO

    // Vertex buffer:
    glGenBuffers(1, &mesh.vbo); // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW); // send vertex data to the GPU

    // Create vertex attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (3)
    glEnableVertexAttribArray(0); // enable vertices

    // Normal buffer:
    glGenBuffers(1, &mesh.normalBuffer); // create normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBuffer); // bind normal buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.totalNormals, normals,
                 GL_STATIC_DRAW); // send normal data to GPU

    // Create normal attribute:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (2)
    glEnableVertexAttribArray(1); // enable normals

    // UV buffer:
    glGenBuffers(1, &mesh.uvBuffer); // create UV buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.uvBuffer); // bind UV buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW); // send UV data to GPU

    // Create UV attribute:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (2)
    glEnableVertexAttribArray(2); // enable UVs

    return true;
}

void Plane::render() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    if (mesh.textured) {
        mesh.material.diffuse.bind();
    }

    glBindVertexArray(mesh.vao); // activate the VBOs contained within the mesh's VAO
    glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices); // draw the plane
    glBindVertexArray(0); // unbind vao

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

