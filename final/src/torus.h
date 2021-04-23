// Name: torus.h
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Torus primitive

#ifndef TORUS_H
#define TORUS_H

#include "model.h"
#include <vector>

class Torus : public Model {
public:
    explicit Torus(int mainSegments = 10, int tubeSegments = 20, float mainRadius = 1.0f, float tubeRadius = 0.5f);

    bool init(const char *filename);

    void destroy() override;

    void render() override;

private:
    void build();

    int _mainSegments;
    int _tubeSegments;
    float _mainRadius;
    float _tubeRadius;

    int _numIndices = 0;
    int _primitiveRestartIndex = 0;

    glm::vec3 *vertices;
    glm::vec2 *uvs;
    glm::vec3 *normals;
    GLuint *indices;
};

#endif //TORUS_H
