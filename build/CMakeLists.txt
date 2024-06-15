# using cmake version 3.12
cmake_minimum_required(VERSION 3.12)

# Project Name
project(baregb)

set(DIRECTORY /Users/isaachamann/Repositories/bare_gb/src)
set(GLFW_INCLUDE_DIR /Users/isaachamann/Repositories/bare_gb/dependencies/GLFW/include)
set(GLAD_DIR /Users/isaachamann/Repositories/bare_gb/dependencies/GLAD)
set(GLFW_LIBRARY /Users/isaachamann/Repositories/bare_gb/dependencies/GLFW/lib/libglfw.3.4.dylib)

set(SOURCES ${DIRECTORY}/tests/tests.c ${DIRECTORY}/src/glad.c)
add_executable(baregb ${SOURCES})

target_include_directories(baregb PUBLIC /Users/isaachamann/Repositories/bare_gb/dependencies)

add_library(cpulib SHARED ${DIRECTORY}/include/memory.h ${DIRECTORY}/src/memory.c ${DIRECTORY}/include/cpu.h ${DIRECTORY}/src/cpu.c ${DIRECTORY}/include/ppu.h ${DIRECTORY}/src/ppu.c)

target_include_directories(baregb PUBLIC ${GLFW_INCLUDE_DIR})
# Using the cpu library.

target_link_libraries(baregb cpulib)

target_link_libraries(baregb ${OPENGL_LIBRARIES} ${GLFW_LIBRARY})


