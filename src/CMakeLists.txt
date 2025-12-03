add_executable(${PROJECT_NAME} main.cpp renderer.cpp lolita.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})
find_package(OpenGL REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(${PROJECT_NAME}
    OpenGL::GL
    glfw
    GLEW
    spdlog::spdlog
    stb_image
)

# # For ImGui (assuming Nix installs it as imgui)
find_package(imgui REQUIRED)
target_link_libraries(${PROJECT_NAME} imgui)
# 
