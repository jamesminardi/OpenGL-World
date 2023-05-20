# Projects Settings
cmake_minimum_required (VERSION 3.1)
project (opengl-world)


# The main executable
add_executable(opengl-world
        src/main.cpp
        )

set_property(TARGET opengl-world PROPERTY CXX_STANDARD 23)
target_compile_options(opengl-world PRIVATE -Wall)

set(ONLY_LIBS "ON")
set(glew-cmake_BUILD_SHARED "FALSE")

add_definitions(-DGLEW_STATIC)
add_subdirectory(external/glfw EXCLUDE_FROM_ALL)
add_subdirectory(external/glew EXCLUDE_FROM_ALL)
add_subdirectory(external/glm EXCLUDE_FROM_ALL)

target_link_libraries(opengl-world
        PRIVATE glfw
        PRIVATE libglew_static
        PRIVATE glm
        )

target_include_directories(opengl-world
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
        PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/src
        )