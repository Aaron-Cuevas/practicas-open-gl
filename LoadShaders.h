#ifndef LOAD_SHADERS_H
#define LOAD_SHADERS_H

#include "vgl.h"

typedef struct {
    GLenum      type;
    const char* filename;
    GLuint      shader;
} ShaderInfo;

#ifdef __cplusplus
extern "C" {
#endif
GLuint LoadShaders(const ShaderInfo* shaders);
#ifdef __cplusplus
}
#endif

#endif // LOAD_SHADERS_H
