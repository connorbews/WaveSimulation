cmake_minimum_required(VERSION 3.10)

project(WaveSimulation)

include_directories(
    include
    include/glad
    include/GLFW
    include/glm
    include/glm/detail
    include/glm/gtc
    include/glm/gtx
    include/glm/simd
    include/KHR
    include/nlohmann
    include/stb
)

add_executable(WaveSimulation
    main.cpp
    src/base64.cpp
    src/Camera.cpp
    src/EBO.cpp
    src/glad.c
    src/Loader.cpp
    src/Mesh.cpp
    src/Model.cpp
    src/shaderClass.cpp
    src/Texture.cpp
    src/stb.cpp
    src/VAO.cpp
    src/VBO.cpp
    src/waveModel.cpp
    include/base64.h
    include/Camera.h
    include/EBO.h
    include/Loader.h
    include/Mesh.h
    include/Model.h
    include/shaderClass.h
    include/Texture.h
    include/VAO.h
    include/VBO.h
    include/waveModel.h
)

configure_file(${CMAKE_SOURCE_DIR}/resources/shaders/default.vert ${CMAKE_BINARY_DIR}/resources/shaders/default.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources/shaders/default.frag ${CMAKE_BINARY_DIR}/resources/shaders/default.frag COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources/shaders/initialize.comp ${CMAKE_BINARY_DIR}/resources/shaders/initialize.comp COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources/shaders/light.vert ${CMAKE_BINARY_DIR}/resources/shaders/light.vert COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/resources/shaders/light.frag ${CMAKE_BINARY_DIR}/resources/shaders/light.frag COPYONLY)

target_link_libraries(WaveSimulation
    -lGL
    -lGLU
    ${CMAKE_SOURCE_DIR}/lib/libglfw3.a
    -lfftw3
    -lm
)