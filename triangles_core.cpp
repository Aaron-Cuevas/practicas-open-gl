///////////////////////////////////////////////////////////////////////
//
// circles_core.cpp — draw a colorful circle using a triangle fan.
// Idea sencilla: 1 vértice al centro + N puntos sobre el círculo.
// Cada punto del anillo recibe un color según su ángulo (HSV→RGB).
//
///////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <cmath>
#include "vgl.h"
#include "LoadShaders.h"

using namespace std;

enum VAO_IDs    { CircleVAO, NumVAOs };
enum Buffer_IDs { VBO, NumBuffers };
enum Attrib_IDs { vPosition = 0, vColor = 1 };

static GLuint VAOs[NumVAOs];
static GLuint Buffers[NumBuffers];

static inline void hsv2rgb(float h, float s, float v, float& r, float& g, float& b)
{
    // h in [0,1), s,v in [0,1]
    float i = floorf(h * 6.0f);
    float f = h * 6.0f - i;
    float p = v * (1.0f - s);
    float q = v * (1.0f - f * s);
    float t = v * (1.0f - (1.0f - f) * s);
    int ii = static_cast<int>(i) % 6;
    switch (ii) {
        case 0: r=v; g=t; b=p; break;
        case 1: r=q; g=v; b=p; break;
        case 2: r=p; g=v; b=t; break;
        case 3: r=p; g=q; b=v; break;
        case 4: r=t; g=p; b=v; break;
        case 5: r=v; g=p; b=q; break;
    }
}

// One-time GPU setup
void init(void)
{
    glGenVertexArrays(NumVAOs, VAOs);
    glBindVertexArray(VAOs[CircleVAO]);

    // Genera un abanico: centro + (SEGMENTOS+1) puntos para cerrar el círculo
    const int   SEGMENTOS = 240;
    const float R         = 0.9f;

    // Cada vértice: 2 floats de posición + 3 de color = 5 floats
    std::vector<GLfloat> data;
    data.reserve((SEGMENTOS + 2) * 5);

    // Centro: color neutro (blanco) para que el relleno se degrade suave
    {
        float cx = 0.0f, cy = 0.0f;
        float cr = 1.0f, cg = 1.0f, cb = 1.0f;
        data.insert(data.end(), { cx, cy, cr, cg, cb });
    }

    // Perímetro con colores por ángulo (arco iris)
    const float PI = 3.14159265358979323846f;
    for (int i = 0; i <= SEGMENTOS; ++i) { // <= para cerrar
        float t = static_cast<float>(i) / SEGMENTOS; // 0..1
        float ang = 2.0f * PI * t;
        float x = R * cosf(ang);
        float y = R * sinf(ang);

        float r, g, b;
        hsv2rgb(t, 1.0f, 1.0f, r, g, b);
        data.push_back(x);
        data.push_back(y);
        data.push_back(r);
        data.push_back(g);
        data.push_back(b);
    }

    glGenBuffers(NumBuffers, Buffers);
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[VBO]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), data.data(), GL_STATIC_DRAW);

    // Compila y usa los shaders del círculo
    ShaderInfo shaders[] = {
        { GL_VERTEX_SHADER,   "circle.vert" },
        { GL_FRAGMENT_SHADER, "circle.frag" },
        { GL_NONE,            NULL }
    };
    GLuint program = LoadShaders(shaders);
    glUseProgram(program);

    // Atributo 0: posición (vec2) — stride 5 floats, offset 0
    glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), BUFFER_OFFSET(0));
    glEnableVertexAttribArray(vPosition);

    // Atributo 1: color (vec3) — stride 5 floats, offset 2 floats
    glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), BUFFER_OFFSET(2 * sizeof(GLfloat)));
    glEnableVertexAttribArray(vColor);

    glClearColor(0.f, 0.f, 0.f, 1.f);
}

// Per-frame draw
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBindVertexArray(VAOs[CircleVAO]);

    // Número total de vértices en el VBO = bytes/(5*sizeof(float))
    GLint sizeBytes = 0;
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &sizeBytes);
    GLsizei numVerts = sizeBytes / (5 * sizeof(GLfloat));

    glDrawArrays(GL_TRIANGLE_FAN, 0, numVerts);
}
