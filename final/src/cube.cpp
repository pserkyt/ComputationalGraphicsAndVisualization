// Name: cube.cpp
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Cube primitive with added render to texture and normal mapping features.

#include "cube.h"
#include <stdio.h> // printf

Cube::Cube(float width, float height, float length) {
    this->width = width;
    this->height = height;
    this->length = length;
    renderToTexture = false;
    bufferTexture = nullptr;
}

// Initialize the cube with a texture:
bool Cube::init(const char *filename) {
    if (!renderToTexture) {
        if (!loadTexture(filename)) {
            return false;
        }
    }

    // Vertex data
    mesh.nVertices = 36;
    glm::vec3 verts[36] = {
            // Back:
            glm::vec3(width, -height, -length),
            glm::vec3(-width, -height, -length),
            glm::vec3(width, height, -length),
            glm::vec3(-width, height, -length),
            glm::vec3(width, height, -length),
            glm::vec3(-width, -height, -length),

            // Front:
            glm::vec3(-width, -height, length),
            glm::vec3(width, -height, length),
            glm::vec3(width, height, length),
            glm::vec3(width, height, length),
            glm::vec3(-width, height, length),
            glm::vec3(-width, -height, length),

            // Left:
            glm::vec3(-width, height, length),
            glm::vec3(-width, height, -length),
            glm::vec3(-width, -height, -length),
            glm::vec3(-width, -height, -length),
            glm::vec3(-width, -height, length),
            glm::vec3(-width, height, length),

            // Right:
            glm::vec3(width, height, -length),
            glm::vec3(width, height, length),
            glm::vec3(width, -height, -length),
            glm::vec3(width, -height, length),
            glm::vec3(width, -height, -length),
            glm::vec3(width, height, length),

            // Bottom:
            glm::vec3(-width, -height, -length),
            glm::vec3(width, -height, -length),
            glm::vec3(width, -height, length),
            glm::vec3(width, -height, length),
            glm::vec3(-width, -height, length),
            glm::vec3(-width, -height, -length),

            // Top:
            glm::vec3(width, height, -length),
            glm::vec3(-width, height, -length),
            glm::vec3(width, height, length),
            glm::vec3(-width, height, length),
            glm::vec3(width, height, length),
            glm::vec3(-width, height, -length),
    };

    // UV data:
    mesh.totalUVs = 36;
    glm::vec2 uvs[36] = {
            // Back:
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),

            // Front:
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),

            // Left:
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),

            // Right:
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),

            // Bottom:
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),

            // Top:
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f)
    };

    // Normal data:
    mesh.totalNormals = 36;
    glm::vec3 normals[36] = {
            // Back
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),
            glm::vec3(0.0f, 0.0f, -1.0f),

            // Front:
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 1.0f),

            // Left:
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),
            glm::vec3(-1.0f, 0.0f, 0.0f),

            // Right:
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),

            // Bottom:
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),
            glm::vec3(0.0f, -1.0f, 0.0f),

            // Top:
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
    };

    // Tangent/bitangent data for normal mapping.
    // https://learnopengl.com/Advanced-Lighting/Normal-Mapping
    glm::vec3 tangent[36];
    glm::vec3 bitangent[36];

    // Go through all the normals and calculate the tangent/bitangent:
    for (int i = 0; i < mesh.totalNormals; i += 3) {
        glm::vec3 edge1 = verts[i + 1] - verts[i];
        glm::vec3 edge2 = verts[i + 2] - verts[i];
        glm::vec2 deltaUV1 = uvs[i + 1] - uvs[i];
        glm::vec2 deltaUV2 = uvs[i + 2] - uvs[i];

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        for (int j = 0; j < 3; ++j) {
            tangent[i + j].x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            tangent[i + j].y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            tangent[i + j].z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            bitangent[i + j].x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            bitangent[i + j].y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            bitangent[i + j].z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
        }
    }

    glGenVertexArrays(1, &mesh.vao); // create VAO
    glBindVertexArray(mesh.vao); // bind VAO

    // Vertex buffer:
    glGenBuffers(1, &mesh.vbo); // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.nVertices, verts,
                 GL_STATIC_DRAW); // send vertex data to the GPU

    // Create vertex attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(0); // enable vertices

    // Normal buffer:
    glGenBuffers(1, &mesh.normalBuffer); // create normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBuffer); // bind normal buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.totalNormals, normals,
                 GL_STATIC_DRAW); // send normal data to GPU

    // Create normal attribute:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(1); // enable normals

    // UV buffer:
    glGenBuffers(1, &mesh.uvBuffer); // create UV buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.uvBuffer); // bind UV buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.totalUVs, uvs, GL_STATIC_DRAW); // send UV data to GPU

    // Create UV attribute:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(2); // enable UVs

    // Tangent buffer:
    glGenBuffers(1, &mesh.tangentBuffer); // create tangent buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.tangentBuffer); // bind tangent buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.totalNormals, tangent,
                 GL_STATIC_DRAW); // send tangent data to GPU

    // Create tangent attribute:
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(3); // enable tangents

    // Bitangent buffer:
    glGenBuffers(1, &mesh.bitangentBuffer); // create bitangent buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.bitangentBuffer); // bind bitangent buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.totalNormals, bitangent,
                 GL_STATIC_DRAW); // send bitangent data to GPU

    // Create bitangent attribute:
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(4); // enable bitangents

    return true;
}

// Initialize the cube using a framebuffer as the texture:
bool Cube::initBuffer(Texture *texture) {
    if (texture == nullptr) {
        printf("ERROR: Texture is NULL!\n");
        return false;
    }
    this->bufferTexture = texture;
    renderToTexture = true;
    return init(nullptr);
}

// Render the cube:
void Cube::render() {
    glEnableVertexAttribArray(0); // vertices
    glEnableVertexAttribArray(1); // normals
    glEnableVertexAttribArray(2); // uvs
    glEnableVertexAttribArray(3); // tangents
    glEnableVertexAttribArray(4); // bitangents

    // Texture:
    if (renderToTexture) {
        bufferTexture->bind();
    } else {
        if (mesh.textured) {
            mesh.material.diffuse.bind();
        }
        if (mesh.material.useNormalMap) {
            mesh.material.normal.bindNormalMap();
        }
    }

    glBindVertexArray(mesh.vao); // activate the VBOs contained within the mesh's VAO
    glDrawArrays(GL_TRIANGLES, 0, mesh.nVertices); // draw the cube
    glBindVertexArray(0); // unbind VAO:

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
}

