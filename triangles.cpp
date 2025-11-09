///////////////////////////////////////////////////////////////////////
//
//
// triangles.cpp
///////////////////////////////////////////////////////////////////////
#include <iostream>
using namespace std;
#include "vgl.h"
#include "LoadShaders.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];
const GLuint NumVertices = 6;

//---------------------------------------------------------------------
//
// init
//
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
}

//---------------------------------------------------------------------
//
// display
//
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAOs[Triangles]);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
}

//---------------------------------------------------------------------
//
// main
//
int main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA);
    glutInitWindowSize(512, 512);

    // macOS soporta hasta OpenGL 4.1; con freeglut esto funciona.
    glutInitContextVersion(4, 1);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutCreateWindow(argv[0]);

    if (glewInit()) {
        cerr << "Unable to initialize GLEW ... exiting" << endl;
        return EXIT_FAILURE;
    }

    // Fondo negro por claridad
    glClearColor(0.f, 0.f, 0.f, 1.f);

    init();
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
