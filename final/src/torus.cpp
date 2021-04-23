// Name: torus.cpp
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Torus primitive

#include "torus.h"

Torus::Torus(int mainSegments, int tubeSegments, float mainRadius, float tubeRadius) {
    vertices = nullptr;
    normals = nullptr;
    uvs = nullptr;
    indices = nullptr;
    _mainSegments = mainSegments;
    _tubeSegments = tubeSegments;
    _mainRadius = mainRadius;
    _tubeRadius = tubeRadius;

}

bool Torus::init(const char *filename) {
    if (!loadTexture(filename)) {
        return false;
    }

    build();

    glGenVertexArrays(1, &mesh.vao); // generate a single vao
    glBindVertexArray(mesh.vao);

    // Vertex buffer:
    glGenBuffers(1, &mesh.vbo); // create vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo); // bind VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.nVertices, vertices,
                 GL_STATIC_DRAW); // send vertex data to the GPU

    // Create vertex attribute:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (3)
    glEnableVertexAttribArray(0); // enable vertices

    // Index buffer:
    glGenBuffers(1, &mesh.indexBuffer); // create index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.indexBuffer); // bind index buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * mesh.nIndices, indices,
                 GL_STATIC_DRAW); // send index data to GPU

    // Normal buffer:
    glGenBuffers(1, &mesh.normalBuffer); // create normal buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.normalBuffer); // bind normal buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh.nVertices, normals,
                 GL_STATIC_DRAW); // send normal data to GPU

    // Create normal attribute:
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (2)
    glEnableVertexAttribArray(1); // enable normals

    // UV buffer:
    glGenBuffers(1, &mesh.uvBuffer); // create UV buffer
    glBindBuffer(GL_ARRAY_BUFFER, mesh.uvBuffer); // bind UV buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * mesh.nVertices, uvs, GL_STATIC_DRAW); // send UV data to GPU

    // Create UV attribute:
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void *) nullptr); // sizeof(float) * (2)
    glEnableVertexAttribArray(2); // enable UVs

    return true;
}

// Clean up:
void Torus::destroy() {
    if (vertices != nullptr) {
        delete[] vertices;
    }
    if (normals != nullptr) {
        delete[] normals;
    }
    if (uvs != nullptr) {
        delete[] uvs;
    }
    if (indices != nullptr) {
        delete[] indices;
    }
}

void Torus::render() {
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    if (mesh.textured) {
        mesh.material.diffuse.bind();
    }

    glBindVertexArray(mesh.vao); // activate the VBOs contained within the mesh's VAO
    glEnable(GL_PRIMITIVE_RESTART);
    glPrimitiveRestartIndex(_primitiveRestartIndex);
    glDrawElements(GL_TRIANGLE_STRIP, _numIndices, GL_UNSIGNED_INT, 0);
    glDisable(GL_PRIMITIVE_RESTART);
    glBindVertexArray(0); // unbind VAO:

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
}

// I did not write this messy code I just edited it to work with my code lol.
// https://www.mbsoftworks.sk/tutorials/opengl4/011-indexed-rendering-torus/
void Torus::build() {
    // Temps:
    float currentMainSegmentAngle = 0.0f;
    int index = 0; // current face

    // Calculate and cache counts of vertices and indices
    mesh.nVertices = (_mainSegments + 1) * (_tubeSegments + 1);
    _primitiveRestartIndex = mesh.nVertices;
    _numIndices = (_mainSegments * 2 * (_tubeSegments + 1)) + _mainSegments - 1; // oh we do have the amount lol
    mesh.nIndices = _numIndices;

    // Allocate data slots:
    vertices = new glm::vec3[mesh.nVertices];
    normals = new glm::vec3[mesh.nVertices];
    uvs = new glm::vec2[mesh.nVertices];
    indices = new GLuint[_numIndices];

    auto mainSegmentAngleStep = glm::radians(360.0f / float(_mainSegments));
    auto tubeSegmentAngleStep = glm::radians(360.0f / float(_tubeSegments));

    // Vertices:
    for (auto i = 0; i <= _mainSegments; i++) {
        // Calculate sine and cosine of main segment angle
        auto sinMainSegment = sin(currentMainSegmentAngle);
        auto cosMainSegment = cos(currentMainSegmentAngle);
        auto currentTubeSegmentAngle = 0.0f;
        for (auto j = 0; j <= _tubeSegments; j++) {
            // Calculate sine and cosine of tube segment angle
            auto sinTubeSegment = sin(currentTubeSegmentAngle);
            auto cosTubeSegment = cos(currentTubeSegmentAngle);

            // Calculate vertex position on the surface of torus
            vertices[index] = glm::vec3(
                    (_mainRadius + _tubeRadius * cosTubeSegment) * cosMainSegment,
                    (_mainRadius + _tubeRadius * cosTubeSegment) * sinMainSegment,
                    _tubeRadius * sinTubeSegment);
            ++index;

            // Update current tube angle
            currentTubeSegmentAngle += tubeSegmentAngleStep;
        }

        // Update main segment angle
        currentMainSegmentAngle += mainSegmentAngleStep;
    }

    // Texture coordinates:
    auto mainSegmentTextureStep = 2.0f / float(_mainSegments);
    auto tubeSegmentTextureStep = 1.0f / float(_tubeSegments);

    index = 0; // reset index counter
    auto currentMainSegmentTexCoordV = 0.0f;
    for (auto i = 0; i <= _mainSegments; i++) {
        auto currentTubeSegmentTexCoordU = 0.0f;
        for (auto j = 0; j <= _tubeSegments; j++) {
            uvs[index] = glm::vec2(currentTubeSegmentTexCoordU, currentMainSegmentTexCoordV);
            ++index;

            currentTubeSegmentTexCoordU += tubeSegmentTextureStep;
        }

        // Update texture coordinate of main segment
        currentMainSegmentTexCoordV += mainSegmentTextureStep;
    }

    // Normals:
    index = 0; // reset again
    currentMainSegmentAngle = 0.0f;
    for (auto i = 0; i <= _mainSegments; i++) {
        // Calculate sine and cosine of main segment angle
        auto sinMainSegment = sin(currentMainSegmentAngle);
        auto cosMainSegment = cos(currentMainSegmentAngle);
        auto currentTubeSegmentAngle = 0.0f;
        for (auto j = 0; j <= _tubeSegments; j++) {
            // Calculate sine and cosine of tube segment angle
            auto sinTubeSegment = sin(currentTubeSegmentAngle);
            auto cosTubeSegment = cos(currentTubeSegmentAngle);

            normals[index] = glm::vec3(cosMainSegment * cosTubeSegment, sinMainSegment * cosTubeSegment,
                                       sinTubeSegment);
            ++index;

            // Update current tube angle
            currentTubeSegmentAngle += tubeSegmentAngleStep;
        }

        // Update main segment angle
        currentMainSegmentAngle += mainSegmentAngleStep;
    }

    // Indices:
    index = 0; // reset again
    GLuint currentVertexOffset = 0;
    for (auto i = 0; i < _mainSegments; i++) {
        for (auto j = 0; j <= _tubeSegments; j++) {
            GLuint vertexIndexA = currentVertexOffset;
            indices[index] = vertexIndexA;
            ++index;

            GLuint vertexIndexB = currentVertexOffset + _tubeSegments + 1;
            indices[index] = vertexIndexB;
            ++index;

            currentVertexOffset++;
        }

        // Don't restart primitive, if it's last segment, rendering ends here anyway
        if (i != _mainSegments - 1) {
            indices[index] = _primitiveRestartIndex;
            ++index;
        }
    }
}
