// Name: cube.h
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Cube primitive with added render to texture and normal mapping features.

#ifndef CUBE_H
#define CUBE_H

#include "model.h"

class Cube : public Model {
public:
    Cube(float width = 0.5f, float height = 0.5f, float length = 0.5f);

    bool init(const char *filename);

    bool initBuffer(Texture *texture);

    void render() override;

private:
    float width;
    float height;
    float length;
    Texture *bufferTexture; // framebuffer texture
    bool renderToTexture; // render to texture?
};

#endif //CUBE_H
