// Name: material.cpp
// Author: Cameron Winningham
// Date: 4/9/21
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Material class

#include "material.h"

Material::Material() {
    specular = glm::vec3(0.5f, 0.5f, 0.5f); // white specular color
    shininess = 0.0f; // default not shiny
    useNormalMap = false;
}

void Material::setSpecular(glm::vec3 specular) {
    this->specular = specular;
}

void Material::setSpecular(float r, float g, float b) {
    setSpecular(glm::vec3(r, g, b));
}

void Material::setShininess(float shininess) {
    this->shininess = shininess;
}

