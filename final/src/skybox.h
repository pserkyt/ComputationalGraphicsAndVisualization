// Name: skybox.h
// Author: Cameron Winningham
// Date: 4/13/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: SkyBox class

#ifndef SKYBOX_H
#define SKYBOX_H

#include "opengl.h"
#include "texture.h"
#include "shader.h"
#include "camera.h"

#include <vector>

enum {
    SKYBOX_FRONT, SKYBOX_BACK, SKYBOX_LEFT, SKYBOX_RIGHT, SKYBOX_UP, SKYBOX_DOWN // SkyBox texture index enumeration
};

class SkyBox {
public:
    SkyBox();

    bool init(const char *directory);

    void render(Camera *camera);

    void destroy();

private:
    bool loadTextures(const char *directory);

    GLuint vertexArrayID;
    Shader *shader;
    Texture textures[6]; // 6 textures for 6 sides

    GLuint vertexBuffer;
    GLuint uvBuffer;
};

#endif //SKYBOX_H

