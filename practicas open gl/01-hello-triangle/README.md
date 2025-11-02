# Hello Triangle — OpenGL + CMake + GLFW + GLEW

Proyecto minimo para abrir una ventana y dibujar un triangulo con OpenGL Core. Guia corta para comenzar con CMake.

## Requisitos
- CMake >= 3.15
- Compilador C++17 (clang++ o g++)
- Bibliotecas de desarrollo: GLFW y GLEW

macOS (Homebrew):
```bash
brew install cmake glfw glew
```

Debian/Ubuntu:
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential libglfw3-dev libglew-dev
```

## Estructura
```
01-hello-triangle/
├─ CMakeLists.txt
├─ README.md
└─ src/
   └─ main.cpp
```

## Compilacion y ejecucion

macOS:
```bash
mkdir -p build && cd build
cmake -DCMAKE_PREFIX_PATH=/opt/homebrew ..
cmake --build . -j
./hello_triangle
```

Linux:
```bash
mkdir -p build && cd build
cmake ..
cmake --build . -j
./hello_triangle
```

Notas: en macOS suele usarse #version 410 core; en Linux/Windows #version 330 core.

## Que hace cada archivo
- CMakeLists.txt: define el proyecto, busca OpenGL/glfw3/GLEW y enlaza el ejecutable.
- src/main.cpp: inicializa GLFW y GLEW, compila shaders, crea VBO/VAO y dibuja con glDrawArrays.

## Comandos rapidos
macOS:
```bash
mkdir -p build && cd build && cmake -DCMAKE_PREFIX_PATH=/opt/homebrew .. && cmake --build . -j && ./hello_triangle
```
Linux:
```bash
mkdir -p build && cd build && cmake .. && cmake --build . -j && ./hello_triangle
```

## Problemas comunes
- CMake no encuentra GLFW/GLEW: agrega -DCMAKE_PREFIX_PATH=/opt/homebrew en macOS o instala paquetes -dev en Linux.
- Error de version GLSL: usa 410 en macOS y 330 en Linux/Windows.
- Pantalla negra: asegura VAO enlazado antes de dibujar y revisa stride/offsets.
