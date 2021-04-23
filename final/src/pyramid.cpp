// Name: pyramid.cpp
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Pyramid primitive

#include "pyramid.h"

Pyramid::Pyramid(float width, float height) {
    this->width = width;
    this->height = height;
}

bool Pyramid::init(const char *filename) {
    if (!loadTexture(filename)) {
        return false;
    }

    // Only 5 vertices are needed to render a pyramid efficiently by sharing vertex indices:
    GLfloat pyramidVerts[] = {
            0.0f, height / 2, 0.0f, // top center
            -width / 2, -height / 2, width / 2, // bottom left
            width / 2, -height / 2, width / 2, // bottom right
            width / 2, -height / 2, -width / 2, // back right
            -width / 2, -height / 2, -width / 2, // back left
            // bottom 2:
            width / 2, -height / 2, -width / 2, // back right
            width / 2, -height / 2, width / 2, // bottom right
            -width / 2, -height / 2, width / 2, // bottom left
            // bottom 1:
            -width / 2, -height / 2, width / 2, // bottom left
            -width / 2, -height / 2, -width / 2, // back left
            width / 2, -height / 2, -width / 2, // back right
    };

    // UV data:
    GLfloat pyramidUVs[] = {
            0.5f, 1.0f, // top center
            0.0f, 0.0f, // bottom left
            1.0f, 0.0f, // bottom right
            0.0f, 0.0f, // back right
            1.0f, 0.0f, // back left
            // bottom 1:
            1.0f, 0.0f, // bottom left
            1.0f, 1.0f, // back left
            0.0f, 1.0f, // back right
            // bottom 2:
            0.0f, 1.0f, // back right
            0.0f, 0.0f, // bottom right
            1.0f, 0.0f, // bottom left
    };

    // The indices for the pyramid:
    GLushort pyramidIndices[] = {
            0, 1, 2,  // front
            0, 2, 3,  // right
            0, 3, 4,  // back
            0, 4, 1,  // left
            // bottom:
            5, 6, 7,  // bottom left
            8, 9, 10  // bottom right
    };

    // Normals:
    int totalFaces = 6; // total faces
    int totalNormals = 18; // 3 normals per face
    glm::vec3 faceNormals[6]; // one normal per face
    glm::vec3 vertexNormals[18]; // one normal per vertex
    glm::vec3 face[3] = {glm::vec3{0.0f}, glm::vec3{0.0f}, glm::vec3{0.0f}}; // temporary face

    // Zero out normals:
    for (int i = 0; i < totalNormals; ++i) {
        vertexNormals[i] = {0.0f, 0.0f, 0.0f};
    }

    // Calculate normals:
    for (int i = 0; i < totalFaces; ++i) { // for each face
        // First calculate face normals:
        for (int j = 0; j < 3; ++j) { // 3 vertices per face
            int index = pyramidIndices[i * 3 + j]; // index of vertex (face * stride (3) + vertex)
            for (int k = 0; k < 3; ++k) { // 3 floats per vertex
                face[j][k] = pyramidVerts[index * 3 + k]; // fill in polygon vertices
            }
        }
        faceNormals[i] = polygonNormal(face); // get normal from polygon

        // Now set vertex normals:
        for (int j = 0; j < 3; ++j) { // 3 vertices per face
            int index = pyramidIndices[i * 3 + j]; // index of vertex (face * stride (3) + vertex)
            // Already have a normal for this vertex?
            if (vertexNormals[index].x != 0.0f || vertexNormals[index].y != 0.0f || vertexNormals[index].z != 0.0f) {
                // Add it for an averaged normal:
                vertexNormals[index] = normalize(vertexNormals[index] + faceNormals[i]);
            } else {
                vertexNormals[index] = faceNormals[i];
            }
        }
    }

    glGenVertexArrays(1, &mesh.vao); // create VAO
    glBindVertexArray(mesh.vao); // bind VAO

    // Vertex buffer:
    glGenBuffers(1, &mesh.vbo); // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidVerts), pyramidVerts, GL_STATIC_DRAW); // send vertex data to the GPU

    // Create vertex attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (3)
    glEnableVertexAttribArray(0); // enable vertices

    // Index buffer:
    mesh.nIndices = sizeof(pyramidIndices) / sizeof(pyramidIndices[0]);
    glGenBuffers(1, &mesh.indexBuffer); // create index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer); // bind index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pyramidIndices), pyramidIndices,
                 GL_STATIC_DRAW); // send index data to GPU

    // Normal buffer:
    glGenBuffers(1, &mesh.normalBuffer); // create normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBuffer); // bind normal buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexNormals), vertexNormals, GL_STATIC_DRAW); // send normal data to GPU

    // Create normal attribute:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (2)
    glEnableVertexAttribArray(1); // enable normals

    // UV buffer:
    glGenBuffers(1, &mesh.uvBuffer); // create UV buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.uvBuffer); // bind UV buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidUVs), pyramidUVs, GL_STATIC_DRAW); // send UV data to GPU

    // Create UV attribute:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (2)
    glEnableVertexAttribArray(2); // enable UVs

    return true;
}

void Pyramid::render() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    if (mesh.textured) {
        mesh.material.diffuse.bind(); // activate texture
    }

    glBindVertexArray(mesh.vao); // activate the VBOs contained within the mesh's VAO
    glDrawElements(GL_TRIANGLES, mesh.nIndices, GL_UNSIGNED_SHORT, nullptr); // draw the pyramid
    glBindVertexArray(0); // unbind VAO:

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}
