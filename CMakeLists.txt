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

FetchContent_Declare(
    fftw3
    URL https://www.fftw.org/fftw-3.3.10.tar.gz
)

FetchContent_GetProperties(fftw3)

if (NOT fftw3_POPULATED)
    FetchContent_Populate(fftw3)
    add_subdirectory(${fftw3_SOURCE_DIR} ${fftw3_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
)

FetchContent_GetProperties(glm)

if (NOT glm_POPULATED)
    FetchContent_Populate(glm)
    add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
)

FetchContent_GetProperties(nlohmann_json)

if (NOT nlohmann_json_POPULATED)
    FetchContent_Populate(nlohmann_json)
    add_subdirectory(${nlohmann_json_SOURCE_DIR} ${nlohmann_json_BINARY_DIR} EXCLUDE_FROM_ALL)
endif()

add_subdirectory(include)
add_subdirectory(src)

# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}../include/*.h)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(WaveSim main.cpp ${HEADER_FILES} ${SOURCE_FILES})
target_link_libraries(WaveSim PRIVATE glfw)
target_link_libraries(WaveSim PRIVATE fftw3)
target_link_libraries(WaveSim PRIVATE nlohmann_json)
target_link_libraries(WaveSim PRIVATE glm)

install(TARGETS WaveSim DESTINATION bin)