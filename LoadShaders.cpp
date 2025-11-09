#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include "LoadShaders.h"

static std::string ReadFile(const char* path) {
    std::ifstream f(path, std::ios::in | std::ios::binary);
    if (!f) {
        std::cerr << "Cannot open shader file: " << path << "\n";
        return {};
    }
    std::ostringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

GLuint LoadShaders(const ShaderInfo* shaders)
{
    if (!shaders) return 0;

    GLuint program = glCreateProgram();

    for (const ShaderInfo* s = shaders; s->type != GL_NONE; ++s) {
        std::string source = ReadFile(s->filename);
        if (source.empty()) { glDeleteProgram(program); return 0; }

        GLuint shader = glCreateShader(s->type);
        const GLchar* src = source.c_str();
        glShaderSource(shader, 1, &src, nullptr);
        glCompileShader(shader);

        GLint compiled = GL_FALSE;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLchar log[4096];
            glGetShaderInfoLog(shader, sizeof log, nullptr, log);
            std::cerr << "Compile error in " << s->filename << ":\n" << log << "\n";
            glDeleteShader(shader);
            glDeleteProgram(program);
            return 0;
        }
        glAttachShader(program, shader);
    }

    glLinkProgram(program);
    GLint linked = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLchar log[4096];
        glGetProgramInfoLog(program, sizeof log, nullptr, log);
        std::cerr << "Link error:\n" << log << "\n";
        glDeleteProgram(program);
        return 0;
    }
    return program;
}
