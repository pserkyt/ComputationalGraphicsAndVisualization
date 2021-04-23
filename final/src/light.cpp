// Name: light.cpp
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Light classes

#include "light.h"

Light::Light() {
    ambient = glm::vec3(0.2f, 0.2f, 0.2f); // low value
    diffuse = glm::vec3(0.5f, 0.5f, 0.5f); // white light
    specular = glm::vec3(1.0f, 1.0f, 1.0f); // white specular color (shining at full intensity)
}

DirectionalLight::DirectionalLight() {
    // Random direction:
    direction = glm::vec3(-0.2f, -1.0f, -0.3f);
}

PointLight::PointLight() {
    position = glm::vec3(0.0f, 0.0f, 0.0f);
    constant = 1.0f;
    linear = 0.09f;
    quadratic = 0.032f;
}
