/*
Guía breve: “Hello Triangle” con CMake + GLFW + GLEW
====================================================

Objetivo
- Abrir una ventana con un contexto OpenGL moderno.
- Compilar dos shaders (vertex/fragment).
- Subir 3 vértices a la GPU y dibujar un triángulo.

Piezas del pipeline
- GLFW: crea ventana y contexto, recibe eventos.
- GLEW: habilita llamadas modernas a OpenGL (funciones vía punteros).
- Vertex Shader: transforma cada vértice y pasa atributos.
- Fragment Shader: decide el color final de cada fragmento (píxel candidato).
- VBO: buffer con los vértices en la GPU.
- VAO: describe cómo leer los atributos del VBO (stride y offsets).

Compilar y ejecutar
1) mkdir -p build && cd build
2) macOS (Homebrew): cmake -DCMAKE_PREFIX_PATH=/opt/homebrew ..
   Linux (paquetes -dev instalados): cmake ..
3) cmake --build . -j
4) ./hello_triangle
*/

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>

#if defined(APPLE_BUILD)
// macOS publica 4.1 Core; por eso se usa #version 410 aquí.
static const char* VS_SRC =
"#version 410 core\n"
"layout (location=0) in vec3 aPos;"
"layout (location=1) in vec3 aCol;"
"out vec3 vCol;"
"void main(){ vCol=aCol; gl_Position=vec4(aPos,1.0); }";

static const char* FS_SRC =
"#version 410 core\n"
"in vec3 vCol;"
"out vec4 FragColor;"
"void main(){ FragColor=vec4(vCol,1.0); }";
#else
// En Linux/Windows suele bastar 3.3 Core.
static const char* VS_SRC =
"#version 330 core\n"
"layout (location=0) in vec3 aPos;"
"layout (location=1) in vec3 aCol;"
"out vec3 vCol;"
"void main(){ vCol=aCol; gl_Position=vec4(aPos,1.0); }";

static const char* FS_SRC =
"#version 330 core\n"
"in vec3 vCol;"
"out vec4 FragColor;"
"void main(){ FragColor=vec4(vCol,1.0); }";
#endif

// Compila un shader y aborta si hay errores.
static GLuint compile(GLenum type, const char* src){
    GLuint s = glCreateShader(type);
    glShaderSource(s, 1, &src, nullptr);
    glCompileShader(s);
    GLint ok = 0; glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if(!ok){
        char log[2048]; glGetShaderInfoLog(s, 2048, nullptr, log);
        std::fprintf(stderr, "[Shader] %s\n", log);
        std::exit(1);
    }
    return s;
}

// Enlaza vertex+fragment a un programa usable por la GPU.
static GLuint link(GLuint vs, GLuint fs){
    GLuint p = glCreateProgram();
    glAttachShader(p, vs);
    glAttachShader(p, fs);
    glLinkProgram(p);
    GLint ok = 0; glGetProgramiv(p, GL_LINK_STATUS, &ok);
    if(!ok){
        char log[2048]; glGetProgramInfoLog(p, 2048, nullptr, log);
        std::fprintf(stderr, "[Link] %s\n", log);
        std::exit(1);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
    return p;
}

// Ajusta el viewport cuando cambie el tamaño del framebuffer.
static void framebuffer_size(GLFWwindow*, int w, int h){
    glViewport(0, 0, w, h);
}

int main(){
    // 1) Inicializar GLFW y pedir contexto Core 3.3 (4.1 en macOS).
    if(!glfwInit()) return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if defined(APPLE_BUILD)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // 2) Crear ventana y hacer su contexto actual.
    GLFWwindow* win = glfwCreateWindow(800, 600, "Hello Triangle", nullptr, nullptr);
    if(!win){ glfwTerminate(); return 1; }
    glfwMakeContextCurrent(win);
    glfwSetFramebufferSizeCallback(win, framebuffer_size);
    glfwSwapInterval(1); // vsync

    // 3) Inicializar GLEW tras tener contexto activo.
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) return 1;

    // 4) Compilar y enlazar shaders.
    GLuint vs = compile(GL_VERTEX_SHADER, VS_SRC);
    GLuint fs = compile(GL_FRAGMENT_SHADER, FS_SRC);
    GLuint prog = link(vs, fs);

    // 5) Tres vértices con color por vértice: (x,y,z, r,g,b).
    float verts[] = {
        -0.5f,-0.5f,0.0f,  1.f,0.f,0.f,
         0.5f,-0.5f,0.0f,  0.f,1.f,0.f,
         0.0f, 0.5f,0.0f,  0.f,0.f,1.f
    };

    // 6) VAO (layout de atributos) + VBO (datos de vértices).
    GLuint vao=0, vbo=0;
    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // Atributo 0: posición (primeros 3 floats, stride 6 floats).
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Atributo 1: color (siguientes 3 floats, offset 3*sizeof(float)).
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // 7) Bucle: limpiar, dibujar, presentar.
    while(!glfwWindowShouldClose(win)){
        glfwPollEvents();
        glClearColor(0.08f,0.08f,0.10f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(prog);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(win);
    }

    // 8) Limpieza.
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(prog);
    glfwDestroyWindow(win);
    glfwTerminate();
    return 0;
}
