cmake_minimum_required(VERSION 3.15)

# Proyecto mínimo: dibujar un triángulo con OpenGL moderno (Core Profile).
project(hello_triangle CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencias:
# - OpenGL: API principal de render.
# - GLFW: crea ventana y contexto, maneja eventos.
# - GLEW: carga funciones modernas de OpenGL.
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)

# Ejecutable principal.
add_executable(hello_triangle src/main.cpp)

# Enlace a las bibliotecas encontradas.
target_link_libraries(hello_triangle
  PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
)

# En macOS algunos toolchains exigen frameworks explícitos.
if(APPLE)
  target_compile_definitions(hello_triangle PRIVATE APPLE_BUILD)
  target_link_libraries(hello_triangle PRIVATE
    "-framework Cocoa" "-framework IOKit" "-framework CoreVideo"
  )
endif()
