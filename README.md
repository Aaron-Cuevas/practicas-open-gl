# OpenGL Base (GLFW + GLEW) — Dos triángulos


Proyecto mínimo:
- macOS: usa `OpenGL/gl3.h` nativo (sin GLAD) y silencia deprecations.
- Linux/Windows: CMake trae GLAD automáticamente.
- GLFW se obtiene con `FetchContent`.

## Build 
=======
Este proyecto es el esqueleto mínimo  para cualquier aplicación con OpenGL moderno en macOS/Linux/Windows. Dibuja dos triángulos, pero lo importante no es el dibujo, sino **la estructura**: ventana, contexto de OpenGL, carga de funciones, datos en la GPU, shaders, formato de vértices, y el bucle de dibujo.

---

## Por qué **funciona**

Piensa en un teatro:

1. **La ventana** es el escenario donde se ve todo (GLFW la crea).
2. **El contexto de OpenGL** es la electricidad que alimenta las luces y máquinas del escenario (GLFW lo inicializa).
3. **GLEW** es el panel que te da acceso a los interruptores modernos; sin él no puedes encender luces nuevas.
4. **El búfer de vértices (VBO)** es una caja donde guardas tus puntos/triángulos en la **GPU**.
5. **Los shaders** son dos programas muy pequeños que corren en la GPU:
   - **Vertex shader**: coloca cada punto en la pantalla.
   - **Fragment shader**: pinta el color de cada píxel.
6. **La VAO** es una tarjeta laminada que explica “este atributo 0 son 2 floats, empezando en el byte 0…”.
7. **La orden de dibujo** (`glDrawArrays`) es el “acción” del director.

Si todas estas piezas están, la GPU puede **tomar bytes**, **interpretarlos como vértices**, **transformarlos** y **pintarlos**. Por eso funciona.

---

## Por qué es la **base** de otros proyectos

Casi todas las apps con OpenGL repiten la misma receta:

- Crear **ventana + contexto**.
- Cargar **funciones modernas** (GLEW).
- Subir **datos** (vértices, índices, colores, normales, UVs).
- Compilar **shaders** (y luego más: matrices, luces, texturas).
- Describir **formato de atributos** (VAO).
- **Dibujar** cada frame dentro de un bucle.
  
Para hacer algo más complejo, cambias **datos** o **shaders** y agregas pasos estándar:
- Índices (EBO/IBO), **colores** por vértice, **uniformes** (matrices MVP), **texturas**, **depth test**, cámara 3D, entrada de teclado/ratón. La base no cambia.

---

## Diagrama mental simple (pipeline)
CPU (vértices) → VBO en GPU → Vertex Shader → Rasterizador → Fragment Shader → Framebuffer (ventana)
↑                     ↑
VAO        Programa de Shaders (link)
La **VAO** dice cómo leer el VBO. El **programa de shaders** es el pegamento que junta el vertex y el fragment shader.

---

## Qué hace **cada archivo**

- `main_glfw.cpp`  
  Crea la **ventana** y el **contexto** con GLFW, inicializa GLEW, entra al **bucle** de eventos y llama a `init()` y `display()`.

- `triangles_core.cpp`  
  Lógica OpenGL “pura”: crea VAO/VBO, sube vértices, compila y usa shaders, define el formato de atributos y **dibuja**.

- `LoadShaders.h/.cpp`  
  Lee los archivos `.vert` y `.frag`, los compila, reporta errores de compilación y **enlaza** el programa.

- `triangles.vert` / `triangles.frag`  
  Shaders mínimos: posicionan el vértice en pantalla y pintan el color (blanco).

- `vgl.h`  
  Incluye GLEW y define el macro `BUFFER_OFFSET` para punteros en búferes.

- `CMakeLists.txt` y `README.md`  
  Soporte de build y documentación.

---

## Qué hace **cada método** (y por qué)

### En `main_glfw.cpp`

- `glfwInit()`  
  Enciende la “central” de GLFW. Sin esto no hay ventana ni contexto.

- `glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4)`  
  - `…MINOR, 1`, `…CORE_PROFILE`  
  Pides un **contexto moderno** (core) 4.1. En macOS, 4.1 es el tope.

- `glfwCreateWindow(512, 512, "triangles", ...)`  
  Crea la **ventana**. Si falla, no hay escenario.

- `glfwMakeContextCurrent(win)`  
  Le dices a OpenGL: “todas las órdenes van a esta ventana”.

- `glewExperimental = GL_TRUE;`  
  Permite que GLEW cargue extensiones de perfil **core**. En macOS es necesario.

- `glewInit()`  
  Carga los **punteros de función** de OpenGL moderno. Sin esto, muchas funciones valen `nullptr`.

- `init()`  
  Llama a tu configuración de **recursos en GPU** (VAO/VBO/shaders/atributos).

- Bucle principal:
  - `display()`  
    Dibuja el frame: limpiar, bindear VAO, `glDrawArrays`.
  - `glfwSwapBuffers(win)`  
    Intercambia el framebuffer “oculto” con el visible para evitar parpadeo.
  - `glfwPollEvents()`  
    Atiende teclado/ratón/redimensionado.

- `glfwTerminate()`  
  Apaga GLFW y libera recursos del sistema.

### En `triangles_core.cpp`

- `init()`  
  Preparación de la GPU:
  - `glGenVertexArrays / glBindVertexArray`  
    Crea y activa la **VAO**: recordará el formato de atributos.
  - `glGenBuffers / glBindBuffer(GL_ARRAY_BUFFER, ...)`  
    Crea y selecciona el **VBO** para subir datos de vértices.
  - `glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW)`  
    Copia tus vértices desde la RAM a la VRAM.
  - `LoadShaders(...)` y `glUseProgram(program)`  
    Compila/enlaza el **vertex** y **fragment** shader, y activa el programa.
  - `glVertexAttribPointer(vPosition, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0))`  
    Explica “atributo 0 son 2 floats por vértice, pegaditos, empezando en 0”.
  - `glEnableVertexAttribArray(vPosition)`  
    Enciende ese atributo para el pipeline.
  - `glClearColor(0,0,0,1)`  
    Fija el color de limpieza del fondo.

- `display()`  
  Dibujo de cada frame:
  - `glClear(GL_COLOR_BUFFER_BIT)`  
    Limpia la pantalla con el color fijado.
  - `glBindVertexArray(...)`  
    Activa la VAO que describe cómo leer los vértices.
  - `glDrawArrays(GL_TRIANGLES, 0, 6)`  
    Dibuja 6 vértices como dos triángulos.

### En `LoadShaders.cpp`

- Lee archivo → `glCreateShader(type)` → `glShaderSource` → `glCompileShader`.  
  Si hay error, imprime el **log** de compilación.

- Crea programa → `glAttachShader(...)` → `glLinkProgram`.  
  Si falla el link, imprime el **log** de enlace.

- Devuelve el `GLuint` del programa listo para `glUseProgram`.

---

## Cómo construir (macOS, Homebrew)
>>>>>>> 6495e74 (docs: README estilo Feynman (base OpenGL))

```bash
brew install glfw glew
clang++ -std=c++17 triangles_core.cpp main_glfw.cpp LoadShaders.cpp \
  -I. -I"$(brew --prefix)/include" -L"$(brew --prefix)/lib" \
  -lGLEW -lglfw -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo \
  -DGL_SILENCE_DEPRECATION \
  -o triangles_glfw
./triangles_glfw
Cómo construir (Ubuntu/Debian)
sudo apt update && sudo apt install -y build-essential cmake pkg-config \
     libglfw3-dev libglew-dev mesa-common-dev
g++ -std=c++17 triangles_core.cpp main_glfw.cpp LoadShaders.cpp \
   -I. -lGLEW -lglfw -lGL -o triangles_glfw
./triangles_glfw
Siguientes pasos típicos
	1.	EBO/IBO: usa índices para no repetir vértices (glDrawElements).
	2.	Colores / normales / UVs: agrega más atributos y más glVertexAttribPointer.
	3.	Uniformes + matrices: MVP para mover/rotar/escalar.
	4.	Depth test: activa glEnable(GL_DEPTH_TEST) para 3D.
	5.	Texturas: carga imágenes y úsalas en el fragment shader.
	6.	Entrada: con GLFW captura teclado/ratón para cámara libre.

Con estos pasos, este esqueleto se convierte en cualquier app OpenGL que quieras construir.

