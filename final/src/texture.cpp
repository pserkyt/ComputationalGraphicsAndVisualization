// Name: texture.cpp
// Author: Cameron Winningham
// Date: 4/1/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Texture class

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION

#include <stb_image.h>
#include <iostream>

using namespace std;

Texture::Texture() {
    textureID = {nullptr};
    textureTarget = GL_TEXTURE_2D;
    totalTextures = 0;
    width = 800;
    height = 600;
    channels = 4;
    frameBuffer = 0;
    renderBuffer = 0;
    drawBuffers = {nullptr};
}

// Load a texture from a file:
bool Texture::load(const char *filename, unsigned int totalTextures, GLenum textureTarget, GLfloat filterMin,
                   GLfloat filterMag, bool clamp) {
    // Load the image data:
    unsigned char *data = stbi_load(filename, &width, &height, &channels, 0);
    if (data) {
        // OpenGL uses Y coordinates pointing upwards, so flip them:
        flipImageVertically(data, width, height, channels);

        // Create the OpenGL texture:
        bool status = create(data, totalTextures, textureTarget, filterMin, filterMag, clamp, GL_NONE);

        stbi_image_free(data); // free the image data

        return status;
    }

    // Error loading the image
    return false;
}

// Creates a texture from raw data:
bool Texture::create(unsigned char *data, unsigned int totalTextures, GLenum textureTarget, GLfloat filterMin,
                     GLfloat filterMag, bool clamp, GLenum attachment) {
    // Set variables:
    this->totalTextures = totalTextures;
    this->textureTarget = textureTarget;
    this->totalTextures = totalTextures;

    // Initialize the texture IDs:
    textureID = new GLuint[totalTextures];

    // Initialize the texture:
    if (!init(&data, &filterMin, &filterMag, clamp)) {
        return false;
    }

    // Initialize the render target (if applicable)
    initRenderTarget(&attachment);

    glGenerateMipmap(textureTarget); // generate mipmaps

    glBindTexture(textureTarget, 0); // unbind the texture

    return true;
}

void Texture::destroy() {
    if (textureID) {
        glDeleteTextures(totalTextures, textureID);
        delete[] textureID;
    }
    if (frameBuffer) {
        glDeleteFramebuffers(1, &frameBuffer);
    }
    if (renderBuffer) {
        glDeleteRenderbuffers(1, &renderBuffer);
    }
    if (drawBuffers) {
        delete[] drawBuffers;
    }
}

// Binds the texture:
void Texture::bind(unsigned int texture) {
    glActiveTexture(GL_TEXTURE0 + texture);
    glBindTexture(textureTarget, textureID[texture]);
}

// Binds the normal map:
void Texture::bindNormalMap() {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(textureTarget, textureID[0]);
}

// Sets this texture as the render target:
void Texture::bindAsRenderTarget() {
    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(0, 0, width, height);
}

// Initializes an OpenGL texture:
bool Texture::init(unsigned char **data, GLfloat *filterMin, GLfloat *filterMag, bool clamp) {
    glGenTextures(totalTextures, textureID);
    for (int i = 0; i < totalTextures; ++i) {
        glBindTexture(textureTarget, textureID[i]);

        // Clamp to edge? (for skybox textures)
        if (clamp) {
            glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, filterMin[i]);
            glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, filterMag[i]);

            glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(textureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        } else {
            // Set the texture wrapping parameters:
            glTexParameteri(textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Set texture filtering parameters:
            glTexParameteri(textureTarget, GL_TEXTURE_MIN_FILTER, filterMin[i]);
            glTexParameteri(textureTarget, GL_TEXTURE_MAG_FILTER, filterMag[i]);
        }

        if (channels == 3) {
            glTexImage2D(textureTarget, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
        } else if (channels == 4) {
            glTexImage2D(textureTarget, 0, GL_SRGB_ALPHA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
        } else {
            cout << "Not implemented to handle image with " << channels << " channels" << endl;
            return false;
        }
    }

    return true;
}

// Initializes the texture as a render target:
bool Texture::initRenderTarget(GLenum *attachments) {
    if (attachments == nullptr) {
        return false;
    }

    drawBuffers = new GLenum[totalTextures];
    bool hasDepth = false;

    for (int i = 0; i < totalTextures; ++i) {
        if (attachments[i] == GL_DEPTH_ATTACHMENT || attachments[i] == GL_STENCIL_ATTACHMENT) {
            drawBuffers[i] = GL_NONE;
            hasDepth = true;
        }
        drawBuffers[i] = attachments[i];
        if (attachments[i] == GL_NONE) { // no texture attachment?
            continue; // continue to next texture
        }

        // Initialize the framebuffer we will render to:
        if (frameBuffer == 0) {
            glGenFramebuffers(1, &frameBuffer); // generate new frame buffer
            glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer); // bind it
        }

        // Initialize depth render buffer:
        if (!hasDepth) {
            glGenRenderbuffers(1, &renderBuffer); // generate new render buffer
            glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer); // bind it
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
        }

        // Finally, configure the framebuffer:
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachments[i], textureTarget, textureID[i], 0);
    }

    if (frameBuffer != 0) {
        glDrawBuffers(totalTextures, drawBuffers); // set the list of draw buffers

        // Check the framebuffer status:
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            printf("ERROR: FrameBuffer initialization failed!\n");
            assert(false); // crash if debug build
            return false;
        }

        return true;
    }

    return false;
}

// Images are loaded with Y axis going down, but OpenGL's Y axis goes up, so let's flip it.
// This is faster than flipping it in the shader.
void flipImageVertically(unsigned char *image, int width, int height, int channels) {
    for (int j = 0; j < height / 2; ++j) {
        int index1 = j * width * channels;
        int index2 = (height - 1 - j) * width * channels;

        for (int i = width * channels; i > 0; --i) {
            unsigned char tmp = image[index1];
            image[index1] = image[index2];
            image[index2] = tmp;
            ++index1;
            ++index2;
        }
    }
}
