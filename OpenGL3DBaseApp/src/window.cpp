#include <iostream>

#include "window.hpp"
#include "resourceManager.hpp"

//** Private **//
float aspectRatio;

// Resize viewport if user resizes the window
void resizeWindow(GLFWwindow* window, int width, int height) {
    // Sets the viewport up
    // First and second argument are x and y value of the lower left corner of the viewport, usually set to 0, 0
    // Third and 4th argument should be self explanatory
    glViewport(0, 0, width, height);

    // Calculate the new aspect ratio and update the projection matrix (changes when aspect ratio is altered)
    aspectRatio = (float)width / height;
    ResourceManager::giveCamera().updateProjectionMatrix();
}

//** Public **//
GLFWwindow& Window::initialize() {
    unsigned int initialWidth = 800;
    unsigned int initialHeight = 600;
    
    //* Initialize GLFW
    glfwInit();
    // Set a "hint" that will be used by the next call of glfwCreateWindow()
    // The first two calls tell GLFW that we're requesting a minimum OpenGL version of 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // This call tells GLFW that OpenGL will be using the optimized "core profile" (always recommended as legacy OpenGL is straight up worse)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window and store a reference to it. The allocated memory will be freed again at the end of the main() function via glfwTerminate().
    // First two arguments are width and height, third argument is the window's desired "title" (= the name displayed at the top)
    // 4th argument can be used to assign a monitor which will then run the window in fullscreen mode. We don't want that for now, so we pass nullptr.
    // 5th argumemt can be used to share resources with another window. We don't need that for now, so we pass nullptr.
    GLFWwindow& window = *glfwCreateWindow(initialWidth, initialHeight, "OpenGL3DBaseApp", nullptr, nullptr);
    // Sets the current context to the window we created. Important as all drawing happens on the current context only
    glfwMakeContextCurrent(&window);
    aspectRatio = (float)initialWidth / initialHeight;

    //* Initialize GLAD
    // gladLoadGLLoader() is used to configure GLAD with an OpenGL Loader (like GLFW)
    // glfwGetProcAdress() is a GLFW function that returns a function pointer to an OpenGL function for a given name
    // GLAD uses this to query all the needed function calls to the right OpenGL version
    // If gladLoadGLLoader returns 0, something has gone wrong and we log the error
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Error: Failed to initialize GLAD\n" << std::endl;
    }

    // Sets the viewport up
    // First and second argument are x and y value of the lower left corner of the viewport, usually set to 0, 0
    // Third and 4th argument should be self explanatory
    glViewport(0, 0, initialWidth, initialHeight);

    // Register a callback to our function resizeWindow() if the user resizes the window so that the viewport will be updated as well
    glfwSetFramebufferSizeCallback(&window, resizeWindow);

    return window;
}

void Window::getMonitorScreenSize(unsigned int& width, unsigned int& height) {
    // Fetch the current monitor
    GLFWmonitor& monitor = *glfwGetPrimaryMonitor();
    // Fetch the videomode from the monitor. It contains the correct values for width and height
    const GLFWvidmode& vidmode = *glfwGetVideoMode(&monitor);
    width = vidmode.width;
    height = vidmode.height;
}

float Window::getAspectRatio() {
    return aspectRatio;
}