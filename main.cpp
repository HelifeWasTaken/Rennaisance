#include "renn/Window.hpp"
#include "renn/Shader.hpp"

#include "external/Viewer.hpp"
#include "external/Bunny.hpp"

#include <iostream>

#include "renn/ShaderToyRenderer.hpp"

static const int width = 800;
static const int height = 600;

static const float DEFAULT_VIEW_POINT[3] = {5, 5, 5};
static const float DEFAULT_VIEW_CENTER[3] = {0, 0, 0};
static const float DEFAULT_UP_VECTOR[3] = {0, 1, 0};

glm::vec3 viewPoint(DEFAULT_VIEW_POINT[0], DEFAULT_VIEW_POINT[1], DEFAULT_VIEW_POINT[2]);
glm::vec3 viewCenter(DEFAULT_VIEW_CENTER[0], DEFAULT_VIEW_CENTER[1], DEFAULT_VIEW_CENTER[2]);
glm::vec3 upVector(DEFAULT_UP_VECTOR[0], DEFAULT_UP_VECTOR[1], DEFAULT_UP_VECTOR[2]);

float aspect = (float) width / (float) height;
Viewer viewer = Viewer(viewPoint, viewCenter, upVector, 45.0f, aspect);

glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, 0.0f));
glm::mat4 rotate = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 0.5) * 4.0f);
glm::mat4 model = translate * rotate * scale; // Combination of transformation matrix

glm::vec3 eye = viewer.getViewPoint();
glm::vec3 look = viewer.getViewCenter();
glm::vec3 up = viewer.getUpVector();
glm::mat4 view = glm::lookAt(eye, look, up); // Calculate view matrix from paramters of viewer

glm::mat4 projection = glm::perspective(45.0f, 1.0f * (float) width / (float) height, 0.1f, 500.0f);
glm::mat4 mView = view * model;
glm::mat4 mvp = projection * view * model;

glm::vec4 lightPos(30, 50, 50, 1); // light position

glm::vec3 Ka(0.1, 0.1, 0.1); // Ambient Object Color
glm::vec3 Kd(1, 1, 0); // Diffuse Object Color
glm::vec3 Ks(1, 1, 1); // Specular Object Color

glm::vec3 lightIntensity = glm::vec3(1, 1, 1); // La * Ld * Ls

glm::mat4 inverseMVP = glm::inverse(mView);
glm::mat3 normalMatrix = glm::mat3(glm::transpose(inverseMVP)); // normal matrix
GLfloat shininess = 120.0f;

static void calculateAllAgain()
{
    eye = viewer.getViewPoint();
    look = viewer.getViewCenter();
    up = viewer.getUpVector();
    view = glm::lookAt(eye, look, up); // Calculate view matrix from paramters of viewer

    mView = view * model;
    mvp = projection * view * model;

    inverseMVP = glm::inverse(mView);
    normalMatrix = glm::mat3(glm::transpose(inverseMVP)); // normal matrix
}

bool lButtonDown;
bool rButtonDown;
bool mButtonDown;
double lastMouseX;
double lastMouseY;
double cx, cy;

renn::Window app("Hello World!", glm::ivec2(800, 600));

static void window_size_callback(GLFWwindow *window, int width, int height)
{
    app.setSize(glm::ivec2(width, height));
    viewer.setAspectRatio((float) width / (float) height);
}

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    cx = xpos;
    cy = ypos;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        lastMouseX = xpos;
        lastMouseY = ypos;
    }

    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if (action == GLFW_PRESS)
                lButtonDown = true;
            else if (action == GLFW_RELEASE)
                lButtonDown = false;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if (action == GLFW_PRESS)
                rButtonDown = true;
            else if (action == GLFW_RELEASE)
                rButtonDown = false;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            if (action == GLFW_PRESS)
                mButtonDown = true;
            else if (action == GLFW_RELEASE)
                mButtonDown = false;
            break;
        default:
            break;
    }
}

void mouseDragging(double width, double height)
{
    if (lButtonDown) {
        float fractionChangeX = static_cast<float>(cx - lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(lastMouseY - cy) / static_cast<float>(height);
        viewer.rotate(fractionChangeX, fractionChangeY);
    } else if (mButtonDown) {
        float fractionChangeX = static_cast<float>(cx - lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(lastMouseY - cy) / static_cast<float>(height);
        viewer.zoom(fractionChangeY);
    } else if (rButtonDown) {
        float fractionChangeX = static_cast<float>(cx - lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(lastMouseY - cy) / static_cast<float>(height);
        viewer.translate(-fractionChangeX, -fractionChangeY, 1);
    }
    lastMouseX = cx;
    lastMouseY = cy;
}

int main()
{
    try {
        glViewport(0, 0, app.getSize().x, app.getSize().y);
        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LESS);

        app.setCallbacks(key_callback, cursor_pos_callback, mouse_button_callback, nullptr);

        /*
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        renn::Shader shader(
            renn::buildShader(
            renn::load_file("../shaders/Phong/simple.vert"), GL_VERTEX_SHADER,
            renn::load_file("../shaders/Phong/simple.frag"), GL_FRAGMENT_SHADER),
            {"LightPosition", "Ka", "Kd", "Ks", "LightIntensity", "Shininess", "ModelViewMatrix", "NormalMatrix", "MVP"});
        std::cout << "Shader constructed" << std::endl;

        shader.use();

        auto updateUniforms = [&shader]() {
            shader.setUniform("LightPosition", &lightPos, 1);
            shader.setUniform("Ka", &Ka, 1);
            shader.setUniform("Kd", &Kd, 1);
            shader.setUniform("Ks", &Ks, 1);
            shader.setUniform("LightIntensity", &lightIntensity, 1);
            shader.setUniform("Shininess", &shininess, 1);
            shader.setUniform("ModelViewMatrix", &mView, 1);
            shader.setUniform("NormalMatrix", &normalMatrix, 1);
            shader.setUniform("MVP", &mvp, 1);
        };

        updateUniforms();

        Bunny bunny;
        */
       renn::ShaderToyRenderer renderer(app, renn::load_file("../shaders/Shadertoy/Happy.frag"));

        while (!app.shouldClose()) {
            app.pollEvents();
            app.clear();
            //bunny.draw();
            //calculateAllAgain();
            //updateUniforms();
            mouseDragging(app.getSize().x, app.getSize().y);
            renderer.Update(app, glm::vec2(cx, cy), lButtonDown, rButtonDown);
            renderer.Render();

            app.render();
        }
        return 0;
    } catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }
}