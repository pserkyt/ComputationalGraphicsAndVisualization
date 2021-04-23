// Name: light.h
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Light classes

#ifndef LIGHT_H
#define LIGHT_H

#include "opengl.h"
#include "geometry.h"

class Light {
public:
    Light();

    glm::vec3 diffuse; // diffuse color
    glm::vec3 ambient; // ambient color
    glm::vec3 specular; // specular color
};

class DirectionalLight : public Light {
public:
    DirectionalLight();

    glm::vec3 direction;
};

class PointLight : public Light {
public:
    PointLight();

    glm::vec3 position;
    float constant;
    float linear;
    float quadratic;
};

#endif //LIGHT_H
