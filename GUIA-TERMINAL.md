# Guía de comandos — Terminal->OpenGL->GitHub

## 1) Navegación
```
pwd
ls -la
cd "/ruta con espacios"
cd ..
```

## 2) Crear estructura y archivos
```
mkdir -p "practicas open gl/01-hello-triangle"/{src,build}
cat > "practicas open gl/01-hello-triangle/CMakeLists.txt" <<EOF
cmake_minimum_required(VERSION 3.15)
project(hello_triangle CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(OpenGL REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(GLEW REQUIRED)
add_executable(hello_triangle src/main.cpp)
target_link_libraries(hello_triangle PRIVATE OpenGL::GL glfw GLEW::GLEW)
if(APPLE)
  target_compile_definitions(hello_triangle PRIVATE APPLE_BUILD)
  target_link_libraries(hello_triangle PRIVATE "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()
EOF
cat > "practicas open gl/01-hello-triangle/src/main.cpp" <<EOF
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <cstdlib>
#if defined(APPLE_BUILD)
static const char* VS_SRC="#version 410 core\nlayout(location=0) in vec3 aPos;layout(location=1) in vec3 aCol;out vec3 vCol;void main(){vCol=aCol;gl_Position=vec4(aPos,1.0);}";
static const char* FS_SRC="#version 410 core\nin vec3 vCol;out vec4 FragColor;void main(){FragColor=vec4(vCol,1.0);}";
#else
static const char* VS_SRC="#version 330 core\nlayout(location=0) in vec3 aPos;layout(location=1) in vec3 aCol;out vec3 vCol;void main(){vCol=aCol;gl_Position=vec4(aPos,1.0);}";
static const char* FS_SRC="#version 330 core\nin vec3 vCol;out vec4 FragColor;void main(){FragColor=vec4(vCol,1.0);}";
#endif
static GLuint compile(GLenum t,const char* s){GLuint x=glCreateShader(t);glShaderSource(x,1,&s,nullptr);glCompileShader(x);GLint ok=0;glGetShaderiv(x,GL_COMPILE_STATUS,&ok);if(!ok){char log[2048];glGetShaderInfoLog(x,2048,nullptr,log);std::fprintf(stderr,"%s\n",log);std::exit(1);}return x;}
static GLuint link(GLuint vs,GLuint fs){GLuint p=glCreateProgram();glAttachShader(p,vs);glAttachShader(p,fs);glLinkProgram(p);GLint ok=0;glGetProgramiv(p,GL_LINK_STATUS,&ok);if(!ok){char log[2048];glGetProgramInfoLog(p,2048,nullptr,log);std::fprintf(stderr,"%s\n",log);std::exit(1);}glDeleteShader(vs);glDeleteShader(fs);return p;}
static void fb(GLFWwindow*,int w,int h){glViewport(0,0,w,h);} 
int main(){if(!glfwInit())return 1;glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
#if defined(APPLE_BUILD)
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
#endif
GLFWwindow* w=glfwCreateWindow(800,600,"Hello Triangle",nullptr,nullptr);if(!w){glfwTerminate();return 1;}glfwMakeContextCurrent(w);glfwSetFramebufferSizeCallback(w,fb);glfwSwapInterval(1);glewExperimental=GL_TRUE;if(glewInit()!=GLEW_OK)return 1;GLuint vs=compile(GL_VERTEX_SHADER,VS_SRC),fs=compile(GL_FRAGMENT_SHADER,FS_SRC),prog=link(vs,fs);float v[]={-0.5f,-0.5f,0.f,1.f,0.f,0.f, 0.5f,-0.5f,0.f,0.f,1.f,0.f, 0.f,0.5f,0.f,0.f,0.f,1.f};GLuint vao=0,vbo=0;glGenVertexArrays(1,&vao);glGenBuffers(1,&vbo);glBindVertexArray(vao);glBindBuffer(GL_ARRAY_BUFFER,vbo);glBufferData(GL_ARRAY_BUFFER,sizeof(v),v,GL_STATIC_DRAW);glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)0);glEnableVertexAttribArray(0);glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6*sizeof(float),(void*)(3*sizeof(float)));glEnableVertexAttribArray(1);while(!glfwWindowShouldClose(w)){glfwPollEvents();glClearColor(0.08f,0.08f,0.10f,1.0f);glClear(GL_COLOR_BUFFER_BIT);glUseProgram(prog);glBindVertexArray(vao);glDrawArrays(GL_TRIANGLES,0,3);glSwapBuffers(w);}glDeleteBuffers(1,&vbo);glDeleteVertexArrays(1,&vao);glDeleteProgram(prog);glfwDestroyWindow(w);glfwTerminate();return 0;}
EOF
```

## 3) Compilar con CMake
```
cmake -S "practicas open gl/01-hello-triangle" -B "practicas open gl/01-hello-triangle/build" -DCMAKE_PREFIX_PATH="$(brew --prefix)"
cmake --build "practicas open gl/01-hello-triangle/build" -j
"practicas open gl/01-hello-triangle/build/hello_triangle"
```

## 4) Git y PR
```
git init -b main
git add -A
git commit -m "add 01-hello-triangle"
git switch -c feat/guia-terminal
```

## 5) Mover imagen al README
```
mkdir -p "practicas open gl/01-hello-triangle/images"
mv "/Users/star/Downloads/triangle.png" "practicas open gl/01-hello-triangle/images/triangle.png"
printf "\n## Ejecucion\n\n![Salida](images/triangle.png)\n" >> "practicas open gl/01-hello-triangle/README.md"
```
