// Disclaimer: I got most of this code from https://learnopengl.com which is a fantastic resource for starting out

#include <iostream>

#include "window.hpp"

float aspectRatio;

GLFWwindow* initializeWindow(int* resCode) {
    // Set basic settings
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window and save current aspectRatio
    GLFWwindow* window = glfwCreateWindow(800, 600, "Slingshot", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        *resCode = -1;
    }
    glfwMakeContextCurrent(window);
    aspectRatio = 800.0f / 600;

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        *resCode = -1;
    }

    // Initialize OpenGL viewport
    glViewport(0, 0, 800, 600);

    // Register a function to resize the rendering viewport if the user resizes the window
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    return window;
}

// Resize viewport if user resizes the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    aspectRatio = (float)width / height;
}

void getScreenSize(int* width, int* height) {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);
    *width = vidmode->width;
    *height = vidmode->height;
}