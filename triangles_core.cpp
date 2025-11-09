#include <iostream>
#include "vgl.h"
#include "LoadShaders.h"

using namespace std;

enum VAO_IDs    { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

static GLuint VAOs[NumVAOs];
static GLuint Buffers[NumBuffers];
static const GLuint NumVertices = 6;

// One-time GPU setup
void init(void)
{
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[Triangles]);

    GLfloat vertices[NumVertices][2] = {
        { -0.90f, -0.90f }, // Triangle 1
        {  0.85f, -0.90f },
        { -0.90f,  0.85f },
        {  0.90f, -0.85f }, // Triangle 2
        {  0.90f,  0.90f },
        { -0.85f,  0.90f }
    };

    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER,   "triangles.vert" },
        { GL_FRAGMENT_SHADER, "triangles.frag" },
        { GL_NONE,            NULL }
    };
    GLuint program = LoadShaders(shaders);
    glUseProgram(program);

    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);

    glClearColor(0.f, 0.f, 0.f, 1.f);
}

// Per-frame draw
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush(); // harmless with GLFW; swap happens in the window loop
}
