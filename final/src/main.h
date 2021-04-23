// Name: main.h
// Author: Cameron Winningham
// Date: 3/27/2021
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Prototypes

#ifndef MAIN_H
#define MAIN_H

#include "model.h"
#include "shader.h"

// Prototypes:
bool initialize(int, char *[], GLFWwindow **window);

void resizeWindow(GLFWwindow *window, int width, int height);

void processInput(GLFWwindow *window);

void mouseCallback(GLFWwindow *window, double xpos, double ypos);

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);

void focusCallback(GLFWwindow *window, int focused);

void renderScene();

void render(Model *model, Shader *shader);

void bindWindowRenderTarget();

#endif //MAIN_H
