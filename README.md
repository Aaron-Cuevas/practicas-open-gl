# OpenGL Quickstart (GLFW + macOS nativo / GLAD en otros)

Proyecto mínimo, rápido y sin drama:
- macOS: usa `OpenGL/gl3.h` nativo (sin GLAD) y silencia deprecations.
- Linux/Windows: CMake trae GLAD automáticamente.
- GLFW se obtiene con `FetchContent`.

## Build rápido

```bash
# Requisitos mínimos
# macOS: Xcode CLT y CMake instalado (brew install cmake) 
# Linux: g++, cmake, xorg-dev y demás (dependen de la distro)
# Windows: MSVC + CMake

# Compilación
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . -j

# Ejecutar
./bin/opengl_quickstart
```

Notas:
- En macOS se pide contexto 4.1 core y `GLFW_OPENGL_FORWARD_COMPAT`.
- Evitamos nombres como `GL_MIN` para no chocar con defines de `gl3.h`.
- VSync activado por defecto (`glfwSwapInterval(1)`).

## Estructura
```
src/main.cpp
CMakeLists.txt
shaders/        (vacío por ahora; shaders embebidos en el binario)
scripts/build.sh
```

## Problemas comunes
- **Undefined symbols con OpenGL en macOS**: asegúrate de compilar con este CMake (linkea frameworks).
- **GL versión vieja**: tu GPU/driver puede limitar el perfil; baja a 3.3 si es necesario.
- **Wayland (Linux)**: instala paquetes X11/Wayland dev según tu entorno.
