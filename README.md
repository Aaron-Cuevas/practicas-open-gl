# OpenGL Base (GLFW + GLEW) — ¿Por qué funciona?

Este proyecto es un esqueleto **mínimo y correcto** para apps con OpenGL moderno. Ahora mismo dibuja un **círculo de colores** con un triángulo-abanico. Lo importante no es el dibujo, sino la **estructura** que toda app real necesita.

---

## Por qué funciona (explicado simple)

Imagina un teatro:

1) **Ventana + Contexto**: la ventana es el escenario; el *contexto de OpenGL* es la electricidad. GLFW enciende las luces y te da un escenario donde OpenGL puede trabajar.

2) **GLEW**: es la caja de herramientas moderna. Sin GLEW, muchas funciones de OpenGL serían punteros nulos. Lo activamos con `glewExperimental = GL_TRUE` y `glewInit()`.

3) **Datos en la GPU (VBO)**: los vértices viven en un **Vertex Buffer Object**. Ponerlos en la GPU evita estar mandando listas desde la CPU cada cuadro.

4) **VAO**: una **Vertex Array Object** es una tarjeta laminada con el “formato” de tus vértices: qué atributo va dónde, cuántos floats, qué *stride* y *offset*.

5) **Shaders**: dos programitas en la GPU.
   - **Vertex shader**: recibe cada vértice y lo coloca en la pantalla.
   - **Fragment shader**: decide el color de cada píxel.
   Los compilamos, verificamos errores y los enlazamos en un programa.

6) **Dibujo**: con todo listo, basta un `glDrawArrays(...)`. La GPU toma los bytes del VBO, los interpreta según la VAO, corre los shaders y pinta.

Cambia **datos** y dibujas otra geometría. Cambia **shaders** y cambias la apariencia o las transformaciones. La receta base es la misma.

---

## Pipeline mental en una línea
 v
CPU (vértices) → VBO (GPU) → [VAO: formato]
→ Vertex Shader → Rasterizador → Fragment Shader → Framebuffer (ventana)
---

## Qué hace cada archivo (lo mínimo para vivir)

- **`main_glfw.cpp`**  
  Crea ventana y *contexto* (GLFW), inicializa GLEW, entra al bucle y llama a `init()` y `display()` cada cuadro.

- **`triangles_core.cpp`**  
  Lógica OpenGL pura:
  - Crea VAO y VBO.
  - Genera vértices del círculo (centro + anillo) y color por ángulo.
  - Compila `circle.vert` y `circle.frag`.
  - Define atributos: posición en `location=0`, color en `location=1`.
  - En `display()` limpia y dibuja con `GL_TRIANGLE_FAN`.

- **`LoadShaders.h/.cpp`**  
  Lee archivos, compila shaders, imprime logs de error, enlaza el programa y entrega el `GLuint` listo para `glUseProgram`.

- **`circle.vert` / `circle.frag`**  
  Shaders 4.1 core. El vertex pasa posición y color; el fragment pinta el color.

- **`vgl.h`**  
  Incluye GLEW y define `BUFFER_OFFSET` para offsets de puntero en buffers.

---

## Flujo paso a paso

1) **GLFW**: `glfwInit()` → hints de versión/perfil → `glfwCreateWindow()` → `glfwMakeContextCurrent()`.  
2) **GLEW**: `glewExperimental = GL_TRUE; glewInit();`  
3) **GPU Setup (init)**:
   - `glGenVertexArrays/glBindVertexArray`
   - `glGenBuffers/glBindBuffer(GL_ARRAY_BUFFER)`
   - `glBufferData(...)` con los vértices
   - `LoadShaders(...)` → `glUseProgram(...)`
   - `glVertexAttribPointer(...)` + `glEnableVertexAttribArray(...)`
4) **Frame (display)**:
   - `glClear(GL_COLOR_BUFFER_BIT)`
   - `glBindVertexArray(...)`
   - `glDrawArrays(GL_TRIANGLE_FAN, 0, N)`
   - GLFW hace `swapBuffers` en el bucle.
5) **Bucle**: `while (!glfwWindowShouldClose)` → `display()` → `glfwSwapBuffers()` → `glfwPollEvents()`.

---

## Compilar y ejecutar (macOS, Homebrew)

```bash
brew install glfw glew
clang++ -std=c++17 triangles_core.cpp main_glfw.cpp LoadShaders.cpp \
  -I. -I"$(brew --prefix)/include" -L"$(brew --prefix)/lib" \
  -lGLEW -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo \
  -DGL_SILENCE_DEPRECATION \
  -o circle_glfw
./circle_glfw
Compilar y ejecutar (Ubuntu/Debian)
sudo apt update && sudo apt install -y build-essential cmake pkg-config \
     libglfw3-dev libglew-dev mesa-common-dev
g++ -std=c++17 triangles_core.cpp main_glfw.cpp LoadShaders.cpp \
   -I. -lGLEW -lglfw -lGL -o circle_glfw
./circle_glfw
Siguientes pasos típicos
	•	Índices (EBO) y glDrawElements.
	•	Atributos extra: normales, UVs, colores por vértice.
	•	Uniformes (matrices MVP) y cámara.
	•	glEnable(GL_DEPTH_TEST) para 3D.
	•	Texturas y muestreo.
	•	Entrada con GLFW para mover la cámara.

Esta base no cambia: ventana+contexto, funciones modernas, buffers en GPU, shaders, VAO, draw call. Todo lo demás son extensiones naturales.
