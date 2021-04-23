// Name: skybox.cpp
// Author: Cameron Winningham
// Date: 4/13/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: SkyBox class

#include "skybox.h"

SkyBox::SkyBox() {
    vertexArrayID = 0;
    shader = nullptr;
    vertexBuffer = 0;
    uvBuffer = 0;
}

void SkyBox::destroy() {
    glDeleteVertexArrays(1, &vertexArrayID);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteBuffers(1, &uvBuffer);

    // Free textures:
    for (auto &texture : textures) {
        texture.destroy();
    }

    // Free shader:
    shader->destroy();
    delete shader;
}

bool SkyBox::init(const char *directory) {
    // Hard coded :)
    shader = new Shader("../shaderfiles/skybox.vs", "../shaderfiles/skybox.frag");
    if (shader->ID == 0) {
        printf("ERROR: Failed to init SkyBox!\n");
        return false;
    }

    // Set skybox texture to texture unit 0:
    shader->setInt("cubemapTexture", 0);

    // Load textures:
    if (!loadTextures(directory)) {
        printf("ERROR: Failed to load SkyBox textures!\n");
        return false;
    }

    // Skybox size: (10 sounds fine :)
    float s = 10.0f;

    // Vertex data:
    glm::vec3 skyboxVertices[24] = {
            // Front face
            glm::vec3(-s, s, -s), glm::vec3(-s, -s, -s), glm::vec3(s, s, -s), glm::vec3(s, -s, -s),
            // Back face
            glm::vec3(s, s, s), glm::vec3(s, -s, s), glm::vec3(-s, s, s), glm::vec3(-s, -s, s),
            // Left face:
            glm::vec3(-s, s, s), glm::vec3(-s, -s, s), glm::vec3(-s, s, -s), glm::vec3(-s, -s, -s),
            // Right face:
            glm::vec3(s, s, -s), glm::vec3(s, -s, -s), glm::vec3(s, s, s), glm::vec3(s, -s, s),
            // Top face:
            glm::vec3(-s, s, -s), glm::vec3(s, s, -s), glm::vec3(-s, s, s), glm::vec3(s, s, s),
            // Bottom face:
            glm::vec3(s, -s, -s), glm::vec3(-s, -s, -s), glm::vec3(s, -s, s), glm::vec3(-s, -s, s),
    };

    // UV data:
    glm::vec2 skyboxTexCoords[24] = {
            glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 1.0f)
    };

    // Create VAO for skybox:
    glGenVertexArrays(1, &vertexArrayID);
    glBindVertexArray(vertexArrayID);

    // VB:
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 24, skyboxVertices, GL_STATIC_DRAW);

    // Vertex pointer:
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(0); // enable vertices (not needed?)

    // UB:
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 24, skyboxTexCoords, GL_STATIC_DRAW);

    // UV pointer:
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void *) nullptr);
    glEnableVertexAttribArray(1); // enable UVs (not needed?)

    return true;
}

bool SkyBox::loadTextures(const char *directory) {
    char filename[255];

    sprintf_s(filename, "%s/up.jpg", directory);
    if (!textures[SKYBOX_UP].load(filename, 1, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, true)) {
        return false;
    }

    sprintf_s(filename, "%s/dn.jpg", directory);
    if (!textures[SKYBOX_DOWN].load(filename, 1, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, true)) {
        return false;
    }

    sprintf_s(filename, "%s/lf.jpg", directory);
    if (!textures[SKYBOX_LEFT].load(filename, 1, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, true)) {
        return false;
    }

    sprintf_s(filename, "%s/rt.jpg", directory);
    if (!textures[SKYBOX_RIGHT].load(filename, 1, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, true)) {
        return false;
    }

    sprintf_s(filename, "%s/bk.jpg", directory);
    if (!textures[SKYBOX_BACK].load(filename, 1, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, true)) {
        return false;
    }

    sprintf_s(filename, "%s/ft.jpg", directory);
    if (!textures[SKYBOX_FRONT].load(filename, 1, GL_TEXTURE_2D, GL_LINEAR, GL_LINEAR, true)) {
        return false;
    }

    return true;
}

void SkyBox::render(Camera *camera) {
    // Bind the shader:
    shader->use();

    // Get and set uniforms:
    shader->setMat4("model", glm::translate(camera->Position));
    shader->setMat4("view", camera->getView());
    shader->setMat4("projection", camera->getProjection());

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // VAO:
    glBindVertexArray(vertexArrayID);

    //glDepthMask(0);

    for (int i = 0; i < 6; i++) {
        textures[i].bind();
        glDrawArrays(GL_TRIANGLE_STRIP, i * 4, 4);
    }

    //glDepthMask(1);

    glBindVertexArray(0); // unbind VAO:

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}


