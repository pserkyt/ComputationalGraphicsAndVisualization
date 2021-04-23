// Name: material.h
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Material class

#ifndef MATERIAL_H
#define MATERIAL_H

#include "texture.h"
#include "geometry.h"

class Material {
public:
    Material();

    void setSpecular(glm::vec3 specular);
    void setSpecular(float r, float g, float b);
    void setShininess(float shininess);

    Texture diffuse; // diffuse (color) texture
    Texture normal; // normal map
    glm::vec3 specular; // specular color
    float shininess; // shininess aka specular power
    bool useNormalMap; // use the normal map?
};

#endif //MATERIAL_H
