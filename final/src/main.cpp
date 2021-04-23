// Name: main.cpp
// Author: Cameron Winningham
// Date: 4/17/2021
// Course ID: CS-330 Comp Graphic and Visualization
// Description: Final

// Standard includes:
#include <iostream> // cout, cerr
#include <cstdlib> // EXIT_FAILURE

// Project includes:
#include "main.h"
#include "sphere.h"
#include "cylinder.h"
#include "plane.h"
#include "cube.h"
#include "pyramid.h"
#include "torus.h"
#include "camera.h"
#include "light.h"
#include "skybox.h"

using namespace std; // using the standard namespace

// Globals:
const char *const WINDOW_TITLE = "Cameron Winningham - Final (press 1 & 2 to change TV channel :)";
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Window:
GLFWwindow *gWindow = nullptr; // GLFW window
bool windowFocused = false; // is the window in focus?

// Camera:
Camera *camera;
float lastX = WINDOW_WIDTH / 2.0f; // last X position of mouse
float lastY = WINDOW_HEIGHT / 2.0f; // last Y position of mouse

// Timing for time-based movement:
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Hall of mirrors effect:
bool hom = true;

// Shaders:
Shader *lights; // shader with lighting
Shader *normalmap; // shader with normal mapping

// Directional light aka sun:
DirectionalLight sun;
//PointLight point; // uncomment in both frag shaders to use point lights

// A globe is made up of a sphere, a pyramid, and a cylinder:
struct Globe {
    Sphere *sphere; // the globe
    Pyramid *pyramid; // base/stand of globe
    Cylinder *cylinder; // globe's rotational axis
};

// A TV consists of many cubes for the outer shell, the screen, and the stand:
struct TV {
    Cube *shell; // outer tv casing
    Cube *screen; // primary screen with hall of mirrors effect
    Cube *scene; // secondary screen with photo of my scene
    Cube *stand; // tv stand
    Cube *base; // tv stand base
};

// A computer consists of two cubes. One for the front face, and one for the outer case:
struct Computer {
    Cube *face; // computer's front face
    Cube *shell; // computer's plastic case
};

// Milestone 1 object:
Globe globe;

// Milestone 2 object:
Plane *table;

// Milestone 3 object:
Torus *donut;

// Final objects:
Computer computer;
TV tv;
Cylinder *drink;
Cylinder *drinkTop;
Cube *keyboard;

// SkyBox, which is way better than looking at a black empty void:
SkyBox *sky;

// Texture to render the scene to using a framebuffer, to create a hall-of-mirrors effect on the TV screen :)
Texture *frameBuffer;

// Program entry:
int main(int argc, char *argv[]) {
    // Initialize:
    if (!initialize(argc, argv, &gWindow)) {
        return EXIT_FAILURE;
    }

    // Init camera:
    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    // Create the shader programs:
    lights = new Shader("../shaderfiles/lights.vs", "../shaderfiles/lights.frag");
    normalmap = new Shader("../shaderfiles/normalmap.vs", "../shaderfiles/normalmap.frag");

    // Allocate shapes:
    globe.sphere = new Sphere(0.6f);
    globe.pyramid = new Pyramid(0.5f, 1.0f);
    globe.cylinder = new Cylinder(0.05f, 0.05f, 1.5f);
    table = new Plane(4.0f, 2.5f);
    donut = new Torus(20, 20, 0.3f, 0.15f);
    computer.face = new Cube(0.2f, 0.6f, 0.1f);
    computer.shell = new Cube(0.22f, 0.62f, 0.8f);
    tv.shell = new Cube(1.5f, 1.0f, 0.1f);
    tv.screen = new Cube(1.3f, 0.8f, 0.05f);
    tv.scene = new Cube(1.3f, 0.8f, 0.05f);
    tv.stand = new Cube(0.5f, 0.6f, 0.1f);
    tv.base = new Cube(1.0f, 0.1f, 0.4f);
    drink = new Cylinder(0.2f, 0.2f, 0.8f);
    drinkTop = new Cylinder(0.2f, 0.2f, 0.01f);
    keyboard = new Cube(1.2f, 0.005f, 0.5f);

    // Allocate SkyBox:
    sky = new SkyBox();

    // Allocate framebuffer:
    frameBuffer = new Texture();

    // Create the framebuffer texture:
    frameBuffer->create(0, 1, GL_TEXTURE_2D, GL_NEAREST, GL_LINEAR, false, GL_COLOR_ATTACHMENT0);

    // Initialize shapes:
    globe.sphere->init("../images/earth.jpeg");
    globe.pyramid->init("../images/cream_marble.jpeg");
    globe.cylinder->init("../images/black_marble.jpeg");
    table->init("../images/dark_wood.jpeg");
    donut->init("../images/donut.bmp");
    computer.face->init("../images/computer.png");
    computer.shell->init("../images/plastic.png");
    tv.shell->init("../images/plastic.png");
    tv.screen->initBuffer(frameBuffer);
    tv.scene->init("../images/scene.jpeg");
    tv.stand->init("../images/plastic.png");
    tv.base->init("../images/plastic.png");
    drink->init("../images/drink.png");
    drinkTop->init("../images/top.png");
    keyboard->init("../images/keyboard.png");

    // Init SkyBox:
    sky->init("../images/miramar");

    // Set object positions and rotations:
    globe.sphere->setRotation(0.0f, 180.0f, 0.0f);
    globe.sphere->setPosition(-2.8f, -0.8f, -4.5f);
    globe.pyramid->setPosition(-2.8f, -1.5f, -4.5f);
    globe.cylinder->setPosition(-2.8f, -0.8f, -4.5f);
    globe.cylinder->setRotation(60.0f, 0.0f, 0.0f);
    table->setPosition(0.0f, -2.0f, -5.0f);
    donut->setPosition(1.6f, -1.8f, -4.0f);
    donut->setRotation(-90.0f, 0.0f, 0.0f);
    computer.face->setPosition(2.5f, -1.38f, -3.78f);
    computer.shell->setPosition(2.5f, -1.38f, -4.5f);
    tv.shell->setPosition(0.0f, -0.6f, -5.0f);
    tv.screen->setPosition(0.0f, -0.6f, -4.94f);
    tv.scene->setPosition(0.0f, -0.6f, -4.94f);
    tv.stand->setPosition(0.0f, -1.5f, -5.2f);
    tv.base->setPosition(0.0f, -2.0f, -5.0f);
    drink->setPosition(-1.8f, -1.6f, -4.2f);
    drink->setRotation(90.0f, 90.0f, 0.0f);
    drinkTop->setPosition(-1.8f, -1.2f, -4.2f);
    drinkTop->setRotation(90.0f, 0.0f, 0.0f);
    keyboard->setPosition(-0.2f, -2.0f, -4.0f);

    // Materials:
    tv.shell->getMaterial()->setShininess(32.0f);
    tv.stand->getMaterial()->setShininess(64.0f);
    tv.base->getMaterial()->setShininess(64.0f);
    computer.shell->getMaterial()->setShininess(32.0f);
    table->getMaterial()->setShininess(32.0f);
    drink->getMaterial()->setShininess(15.0f);
    drinkTop->getMaterial()->setShininess(50.0f);
    keyboard->getMaterial()->setShininess(200.0f);
    globe.sphere->getMaterial()->setShininess(100.0f);

    // Normal maps:
    tv.shell->loadNormalMap("../images/plastic_normal.png");
    tv.stand->loadNormalMap("../images/plastic_normal.png");
    tv.base->loadNormalMap("../images/plastic_normal.png");
    computer.shell->loadNormalMap("../images/plastic_normal.png");

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // black clear color

    glEnable(GL_FRAMEBUFFER_SRGB); // enable gamma correction (textures are loaded in as SRGB)

    // Render loop:
    while (!glfwWindowShouldClose(gWindow)) {
        processInput(gWindow); // get input

        // Per-frame time logic:
        auto currentFrame = (float) glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // First pass, render to framebuffer:
        frameBuffer->bindAsRenderTarget(); // bind framebuffer texture
        renderScene(); // render scene to the framebuffer texture

        // Second pass, render to the window:
        bindWindowRenderTarget(); // bind window to render to
        renderScene(); // render scene as normal to the window

        glfwPollEvents(); // poll events
        glfwSwapBuffers(gWindow); // swap buffers
    }

    // Free shapes:
    globe.sphere->destroy(); // destroy sphere
    delete globe.sphere; // free memory
    globe.pyramid->destroy();
    delete globe.pyramid;
    globe.cylinder->destroy();
    delete globe.cylinder;
    table->destroy();
    delete table;
    donut->destroy();
    delete donut;
    computer.face->destroy();
    delete computer.face;
    computer.shell->destroy();
    delete computer.shell;
    tv.shell->destroy();
    delete tv.shell;
    tv.screen->destroy();
    delete tv.screen;
    tv.scene->destroy();
    delete tv.scene;
    tv.stand->destroy();
    delete tv.stand;
    tv.base->destroy();
    delete tv.base;
    drink->destroy();
    delete drink;
    drinkTop->destroy();
    delete drinkTop;
    keyboard->destroy();
    delete keyboard;

    // Free SkyBox:
    sky->destroy();
    delete sky;

    // Free framebuffer texture:
    frameBuffer->destroy();
    delete frameBuffer;

    // Free shaders:
    lights->destroy();
    delete lights;
    normalmap->destroy();
    delete normalmap;

    // Free camera:
    delete camera;

    glfwTerminate(); // terminate GLFW
    exit(EXIT_SUCCESS); // exit
}

// Initialize GLFW, GLEW, and create a window:
bool initialize(int argc, char *argv[], GLFWwindow **window) {
    glfwInit(); // initialize GLFW

#ifdef __APPLE__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // maximum my Mac supports is 4.1
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else // Windows/Linux:
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
#endif
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the window:
    *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (*window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(*window); // set window current
    glfwSetFramebufferSizeCallback(*window, resizeWindow); // set resize callback function
    glfwSetCursorPosCallback(*window, mouseCallback); // set mouse movement callback
    glfwSetScrollCallback(*window, scrollCallback); // set mouse wheel/scroll callback
    glfwSetWindowFocusCallback(*window, focusCallback); // set window focus callback

    glfwSetInputMode(*window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse

    // Initialize GLEW:
    glewExperimental = GL_TRUE; // for GLEW 1.13 or earlier
    GLenum GlewInitResult = glewInit();
    if (GLEW_OK != GlewInitResult) {
        std::cerr << glewGetErrorString(GlewInitResult) << std::endl;
        return false;
    }

    // Displays GPU OpenGL version:
    cout << "INFO: OpenGL Version: " << glGetString(GL_VERSION) << endl;

    return true;
}

// Process input via GLFW:
void processInput(GLFWwindow *window) {
    static bool p_pressed = false; // keep track of p key so the perspective/ortho doesnt toggle repeatedly

    // Exit:
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }

    // Move camera forward:
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->ProcessKeyboard(FORWARD, deltaTime);
    }

    // Move camera backward:
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    }

    // Strafe camera left:
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->ProcessKeyboard(LEFT, deltaTime);
    }

    // Strafe camera right:
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->ProcessKeyboard(RIGHT, deltaTime);
    }

    // Move camera up:
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        camera->ProcessKeyboard(UP, deltaTime);
    }

    // Move camera down:
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        camera->ProcessKeyboard(DOWN, deltaTime);
    }

    // Run: (not required but I like the feature)
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        camera->run = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        camera->run = false;
    }

    // Toggle perspective/orthographic:
    if (!p_pressed && glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
        camera->toggleOrthographic();
        p_pressed = true; // GLFW_PRESS event, p is pressed!
    } else if (p_pressed && glfwGetKey(window, GLFW_KEY_P) == GLFW_RELEASE) {
        p_pressed = false; // GLFW_RELEASE event, p has been released!
    }

    // Set scene as TV screen:
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
        hom = false;
    }

    // Set hall of mirrors as TV screen:
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
        hom = true;
    }
}

// GLFW mouse movement callback:
void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    // Skip mouse movement to avoid sudden jerky movement when window regains focus:
    if (windowFocused) {
        windowFocused = false; // reset flag
    } else {
        // Get the offset/delta:
        float offsetX = xpos - lastX;
        float offsetY = lastY - ypos; // reversed since y-coordinates go from bottom to top

        // Set previous position to current position:
        lastX = xpos;
        lastY = ypos;

        camera->ProcessMouseMovement(offsetX, offsetY); // camera mouselook
    }
}

// GLFW mouse wheel/scroll callback:
void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    camera->ProcessMouseScroll((float) yoffset);
}

// GLFW window resize callback function:
void resizeWindow(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Functioned called to render a single frame in the scene:
void renderScene() {
    // Clear the frame and z buffers:
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Disable depth testing and culling for skybox:
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Render SkyBox:
    sky->render(camera);

    // Enable depth testing and culling:
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Render globe:
    render(globe.pyramid, lights);
    render(globe.sphere, lights);
    render(globe.cylinder, lights);

    // Render table:
    render(table, lights);

    // Render donut:
    render(donut, lights);

    // Render computer:
    render(computer.face, lights);
    render(computer.shell, normalmap);

    // Render TV:
    render(tv.shell, normalmap);
    if (hom) {
        render(tv.screen, lights); // hall of mirrors TV screen
    } else {
        render(tv.scene, lights); // scene image TV screen
    }
    render(tv.stand, normalmap);
    render(tv.base, normalmap);

    // Render drink:
    render(drink, lights);
    render(drinkTop, lights);

    // Render keyboard:
    render(keyboard, lights);
}

// Render a single model:
void render(Model *model, Shader *shader) {
    // Bind the shader:
    shader->use();

    // Variables for calculating MVP:
    glm::mat4 scale, rotation, translation, m;
    glm::quat rotationQuat;

    // Calculate MVP:
    scale = glm::scale(glm::vec3(model->getScale()));
    rotationQuat = glm::quat(model->getRotation());
    rotation = glm::toMat4(rotationQuat);
    translation = glm::translate(model->getPosition());
    m = translation * rotation * scale;

    // Get and set uniforms:
    shader->setMat4("model", m);
    shader->setMat4("view", camera->getView());
    shader->setMat4("projection", camera->getProjection());

    // Get and set material diffuse texture uniform: (ONLY NEEDS TO BE DONE ONCE FOR EACH TEXTURE UNIT)
    Material *material = model->getMaterial();
    if (model->hasMaterial()) {
        shader->setInt("material.diffuse", 0); // diffuse (color) texture is texture unit 0
        shader->setInt("material.normal", 1); // normal texture is texture unit 1
        //shader->setInt("material.specular", 2); // specular material (not yet implemented) is texture unit 2
        shader->setVec3("material.specular", material->specular);
        shader->setFloat("material.shininess", material->shininess);
    }

    // Normal mapping:
    shader->setBool("material.hasNormalMap", material->useNormalMap);

    // Get and set light uniforms:
    shader->setVec3("dirLight.direction", sun.direction);
    shader->setVec3("dirLight.ambient", sun.ambient);
    shader->setVec3("dirLight.diffuse", sun.diffuse);
    shader->setVec3("dirLight.specular", sun.specular);

    // Other lighting info:
    shader->setVec3("viewPos", camera->getPosition());
    shader->setVec3("lightPos", sun.direction); // hmm need position for sun?

    // White light:
    /*shader->setVec3("pointLights[0].position", point.position);
    shader->setVec3("pointLights[0].ambient", point.ambient);
    shader->setVec3("pointLights[0].diffuse", point.diffuse);
    shader->setVec3("pointLights[0].specular", point.specular);
    shader->setFloat("pointLights[0].constant", point.constant);
    shader->setFloat("pointLights[0].linear", point.linear);
    shader->setFloat("pointLights[0].quadratic", point.quadratic);*/

    // Render:
    model->render();
}

// This is a little trick I learned by experimenting with GLFW to avoid nasty large mouselook jumps when focus to the
// window is regained or occasionally on startup:
void focusCallback(GLFWwindow *window, int focused) {
    if (focused) {
        windowFocused = true; // flag for mouseCallback function so the first mouse move can be skipped
        glfwSetCursorPos(window, lastX, lastY); // window focused, center mouse cursor
    } else {
        windowFocused = false; // the window lost input focus
    }
}

// Bind the window as the render target:
void bindWindowRenderTarget() {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // reset to normal buffer
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT); // reset viewport
}
