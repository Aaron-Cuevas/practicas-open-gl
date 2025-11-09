#include <iostream>
#include "vgl.h"
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

void init(void);
void display(void);

int main() {
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);       // macOS llega a 4.1
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(512, 512, "triangles", nullptr, nullptr);
    if (!win) { std::cerr << "Window creation failed\n"; glfwTerminate(); return 1; }

    glfwMakeContextCurrent(win);

    // GLEW necesita esto en core profile
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW init failed\n";
        return 1;
    }

    init();

    while (!glfwWindowShouldClose(win)) {
        display();
        glfwSwapBuffers(win);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
