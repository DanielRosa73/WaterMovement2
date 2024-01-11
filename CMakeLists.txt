cmake_minimum_required(VERSION 3.10)
project(POGLA)

# Specify the C++ version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include the project header files
set(HEADER_DIR "${PROJECT_SOURCE_DIR}/include")
include_directories("${HEADER_DIR}")

set(EXTERN_DIR "${PROJECT_SOURCE_DIR}/extern")

# Include the headers for GLFW
set(GLFW_DIR "${EXTERN_DIR}/GLFW")
include_directories("${GLFW_DIR}/include")

# Include the headers for GLAD
set(GLAD_DIR "${EXTERN_DIR}/GLAD")
include_directories("${GLAD_DIR}")

# Include the headers for GLM
set(GLM_DIR "${EXTERN_DIR}/GLM")
include_directories("${GLM_DIR}")

# Include the headers for STB
set(STB_DIR "${EXTERN_DIR}/STB")
include_directories("${STB_DIR}")

# Add GLAD to your project
add_library(glad "${GLAD_DIR}/glad.c")

# Define the source files
file(GLOB_RECURSE SOURCES "src/*.cpp" "${STB_DIR}/*.cpp")

# Add the executable
add_executable(POGLA ${SOURCES})

# Find the GLFW library
find_library(GLFW_LIBRARY NAMES glfw3 glfw PATHS "${GLFW_DIR}/lib-vc2022")

# Link the libraries to your executable
target_link_libraries(POGLA ${GLFW_LIBRARY} glad)
