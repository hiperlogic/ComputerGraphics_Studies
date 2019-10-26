cmake_minimum_required(VERSION 3.0.0)
project(CG_Studies VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(00_OpenGL_app_framework OpenGL/00_app_framework/main.cpp)
add_executable(01_OpenGL_plain_triangle OpenGL/01_plain_triangle/main.cpp)
add_executable(02_OpenGL_plain_colored_triangle OpenGL/02_plain_colored_triangle/main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)

# Linux and Unix systems demands the explicit linking of OpenGL
if(UNIX)
    target_link_libraries(00_OpenGL_app_framework PRIVATE GL)
    target_link_libraries(01_OpenGL_plain_triangle PRIVATE GL)
    target_link_libraries(02_OpenGL_plain_colored_triangle PRIVATE GL)
endif(UNIX)

target_link_libraries(00_OpenGL_app_framework PRIVATE GLEW::GLEW glfw glm)
target_link_libraries(01_OpenGL_plain_triangle PRIVATE GLEW::GLEW glfw glm)
target_link_libraries(02_OpenGL_plain_colored_triangle PRIVATE GLEW::GLEW glfw glm)

