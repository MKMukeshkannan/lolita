# RENDERER MODULE
add_library(renderer)
target_sources(renderer PUBLIC FILE_SET CXX_MODULES FILES renderer.cpp)
target_link_libraries(renderer PRIVATE stb_image)

# LOLITA CORE MODULE
add_library(core)
target_sources(core PUBLIC FILE_SET CXX_MODULES FILES lolita.cpp lolita.effects_registry.cpp lolita.core.cpp)
target_link_libraries(core PRIVATE stb_image)

# LOLITA ENTRY POINT MODULE
add_executable(${PROJECT_NAME} main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

find_package(OpenGL REQUIRED)
find_package(spdlog REQUIRED)
find_package(imgui REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL glfw GLEW spdlog::spdlog renderer core imgui)
