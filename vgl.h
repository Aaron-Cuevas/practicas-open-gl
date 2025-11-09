#ifndef VGL_H
#define VGL_H

#include <cstddef>
#include <cstdint>
#include <GL/glew.h>

#ifndef BUFFER_OFFSET
#define BUFFER_OFFSET(i) (reinterpret_cast<const void*>(static_cast<uintptr_t>(i)))
#endif

#endif // VGL_H
