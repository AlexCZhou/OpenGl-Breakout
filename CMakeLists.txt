cmake_minimum_required(VERSION 3.5)

project(OpenGl_Breakout)

set(CMAKE_CXX_STANDARD 14)

# config glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Dependence/glfw)
include_directories(Dependence/glfw/include)
LIST(APPEND LIBS glfw ${GLFW_LIBRARES})

# config glad
set(GLAD_SRC Dependence/glad/src/glad.c)
if(NOT WIN32)
    LIST(APPEND LIBS dl)
endif()
include_directories(Dependence/glad/include)

# config glm
include_directories(Dependence/glm)


file(GLOB SOURCES "Src/*.cpp" "Src/imgui/*.cpp" ${GLAD_SRC})

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(OpenGl_Breakout 
    PRIVATE
        ${PROJECT_SOURCE_DIR}/Include
        ${PROJECT_SOURCE_DIR}/Include/imgui
)
target_link_libraries(OpenGl_Breakout ${LIBS})