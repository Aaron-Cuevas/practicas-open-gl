cmake_minimum_required(VERSION 3.20)
project(opengl_quickstart CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# GLFW (fetched at configure time)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# On macOS use the native OpenGL headers; on other platforms use GLAD
if(APPLE)
  message(STATUS "Building on macOS: using system OpenGL (no GLAD)")
  add_compile_definitions(GL_SILENCE_DEPRECATION)
else()
  message(STATUS "Non-Apple platform detected: fetching GLAD")
  FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v0.1.36
  )
  # Configure GLAD to generate a modern OpenGL loader
  set(GLAD_PROFILE "core" CACHE STRING "" FORCE)
  set(GLAD_API "gl=4.1" CACHE STRING "" FORCE)
  set(GLAD_GENERATOR "c" CACHE STRING "" FORCE)
  FetchContent_MakeAvailable(glad)
endif()

add_executable(opengl_quickstart
  src/main.cpp
)

# Prevent GLFW from including any GL headers for us
target_compile_definitions(opengl_quickstart PRIVATE GLFW_INCLUDE_NONE)

target_link_libraries(opengl_quickstart PRIVATE glfw)

if(NOT APPLE)
  target_link_libraries(opengl_quickstart PRIVATE glad)
endif()

if(APPLE)
  # Required Apple frameworks
  target_link_libraries(opengl_quickstart PRIVATE
    "-framework Cocoa" "-framework IOKit" "-framework CoreVideo" "-framework OpenGL")
endif()

# Output directory
set_property(TARGET opengl_quickstart PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
