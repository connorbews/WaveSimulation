cmake_minimum_required(VERSION 3.15)

project(WaveSimulation)

include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
)

FetchContent_GetProperties(glfw)

if (NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_subdirectory(include/nlohmann_json)
add_subdirectory(include/glm)
add_subdirectory(include)
add_subdirectory(src)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}../include/*.h)

add_executable(WaveSim main.cpp ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(WaveSim PRIVATE glfw)
target_link_libraries(WaveSim PRIVATE fftw3)
target_link_libraries(WaveSim PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(WaveSim PRIVATE glm::glm)

install(TARGETS WaveSim DESTINATION bin)