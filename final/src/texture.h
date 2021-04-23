// Name: texture.h
// Author: Cameron Winningham
// Date: 4/1/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Texture class

#ifndef TEXTURE_H
#define TEXTURE_H

#include "opengl.h"

class Texture {
public:
    Texture();

    bool load(const char *filename, unsigned int totalTextures = 1, GLenum textureTarget = GL_TEXTURE_2D,
              GLfloat filterMin = GL_NEAREST, GLfloat filterMag = GL_LINEAR, bool clamp = false);

    bool create(unsigned char *data, unsigned int totalTextures = 1, GLenum textureTarget = GL_TEXTURE_2D,
                GLfloat filterMin = GL_NEAREST, GLfloat filterMag = GL_LINEAR, bool clamp = false,
                GLenum attachment = GL_NONE);

    void destroy();

    void bind(unsigned int texture = 0);

    void bindNormalMap();

    void bindAsRenderTarget();

private:
    bool init(unsigned char **data, GLfloat *filterMin, GLfloat *filterMag, bool clamp);

    bool initRenderTarget(GLenum *attachments);

    GLuint *textureID;
    GLenum textureTarget;
    unsigned int totalTextures;
    int width;
    int height;
    int channels;
    GLuint frameBuffer;
    GLuint renderBuffer;
    GLenum *drawBuffers;
};

void flipImageVertically(unsigned char *image, int width, int height, int channels);

#endif //TEXTURE_H
