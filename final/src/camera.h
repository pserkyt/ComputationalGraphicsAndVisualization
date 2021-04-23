// Name: camera.h
// Author: Cameron Winningham
// Date: 3/27/2021
// Course ID: CS-330 Comp Graphic and Visualization
// Description: First-person camera class: a modified version of the OpenGLSample's "camera.h"

#ifndef CAMERA_H
#define CAMERA_H

#include "opengl.h"
#include "geometry.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
    Camera();

    explicit Camera(glm::vec3 position, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = -90.0f,
                    float pitch = 0.0f);

    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);

    glm::mat4 getView();

    glm::mat4 getProjection();

    glm::vec3 getPosition();

    void ProcessKeyboard(Camera_Movement direction, float deltaTime);

    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true);

    void ProcessMouseScroll(float yoffset);

    void toggleOrthographic();

    // Camera attributes:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    // Euler angles:
    float Yaw;
    float Pitch;

    // Camera options:
    float MovementSpeed;
    float runSpeed;
    float MouseSensitivity;
    float Zoom;
    bool run;

private:
    void updateCameraVectors();

    // Projection attributes:
    float near, far;
    float aspect;
    bool ortho;
};

#endif
