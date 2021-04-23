// Name: pyramid.h
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Pyramid primitive

#ifndef PYRAMID_H
#define PYRAMID_H

#include "model.h"

// Pyramid class:
class Pyramid : public Model {
public:
    explicit Pyramid(float width = 1.0f, float height = 1.0f);

    bool init(const char *filename);

    void render() override;

private:
    float width;
    float height;
};

#endif //PYRAMID_H
