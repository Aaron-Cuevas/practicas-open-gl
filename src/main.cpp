#include <cstdio>
#include <cstdlib>

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl3.h>   // Core profile headers on macOS
#else
#include <glad/glad.h>    // Loaded at runtime on other platforms
#endif

#include <GLFW/glfw3.h>

static void glfw_error_callback(int code, const char* desc) {
    std::fprintf(stderr, "[GLFW error %d] %s\n", code, desc);
}

static const char* vert_src_mac = R"(#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
out vec3 vCol;
void main() {
    vCol = aCol;
    gl_Position = vec4(aPos, 1.0);
}
)";

static const char* frag_src_mac = R"(#version 410 core
in vec3 vCol;
out vec4 FragColor;
void main() {
    FragColor = vec4(vCol, 1.0);
}
)";

static const char* vert_src_other = R"(#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
out vec3 vCol;
void main() {
    vCol = aCol;
    gl_Position = vec4(aPos, 1.0);
}
)";

static const char* frag_src_other = R"(#version 330 core
in vec3 vCol;
out vec4 FragColor;
void main() {
    FragColor = vec4(vCol, 1.0);
}
)";

static GLuint make_shader(GLenum type, const char* src) {
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetShaderiv(s, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetShaderInfoLog(s, len, nullptr, log.data());
        std::fprintf(stderr, "Shader compile error: %s\n", log.c_str());
        std::exit(EXIT_FAILURE);
    }
    return s;
}

static GLuint make_program(const char* vs, const char* fs) {
    GLuint v = make_shader(GL_VERTEX_SHADER, vs);
    GLuint f = make_shader(GL_FRAGMENT_SHADER, fs);
    GLuint p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);
    glLinkProgram(p);
    glDeleteShader(v);
    glDeleteShader(f);
    GLint ok = 0;
    glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if (!ok) {
        GLint len = 0;
        glGetProgramiv(p, GL_INFO_LOG_LENGTH, &len);
        std::string log(len, '\0');
        glGetProgramInfoLog(p, len, nullptr, log.data());
        std::fprintf(stderr, "Program link error: %s\n", log.c_str());
        std::exit(EXIT_FAILURE);
    }
    return p;
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        std::fprintf(stderr, "Failed to init GLFW\n");
        return EXIT_FAILURE;
    }

#ifdef __APPLE__
    // Request a 4.1 core profile on macOS
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#else
    // Request at least a 3.3 core profile elsewhere
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow* win = glfwCreateWindow(800, 600, "OpenGL Quickstart", nullptr, nullptr);
    if (!win) {
        std::fprintf(stderr, "Failed to create window\n");
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(win);
    glfwSwapInterval(1); // vsync

#ifndef __APPLE__
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::fprintf(stderr, "Failed to load GL with GLAD\n");
        return EXIT_FAILURE;
    }
#endif

    // Geometry: a simple triangle with per-vertex colors
    float verts[] = {
        // positions         // colors
        -0.8f, -0.6f, 0.0f,   1.0f, 0.2f, 0.2f,
         0.0f,  0.8f, 0.0f,   0.2f, 1.0f, 0.2f,
         0.8f, -0.6f, 0.0f,   0.2f, 0.2f, 1.0f,
    };

    GLuint vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

#ifdef __APPLE__
    GLuint prog = make_program(vert_src_mac, frag_src_mac);
#else
    GLuint prog = make_program(vert_src_other, frag_src_other);
#endif

    int fbw = 0, fbh = 0;
    glfwGetFramebufferSize(win, &fbw, &fbh);
    glViewport(0, 0, fbw, fbh);

    glfwSetFramebufferSizeCallback(win, [](GLFWwindow*, int w, int h){
        glViewport(0, 0, w, h);
    });

    while (!glfwWindowShouldClose(win)) {
        if (glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(win, GLFW_TRUE);
        }
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(prog);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(prog);
    glfwDestroyWindow(win);
    glfwTerminate();
    return EXIT_SUCCESS;
}
