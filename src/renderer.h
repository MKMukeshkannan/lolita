#ifndef MY_HEADER_H
#define MY_HEADER_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <expected>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>

#include "stb_image.h"

namespace lolita {
bool LoadTextureFromFile(const char* file_name, GLuint* out_texture, int* out_width, int* out_height);
bool LoadTextureFromMemory(const void* data, size_t data_size, GLuint* out_texture, int* out_width, int* out_height);

class Renderer {
public:
  Renderer() {};
  ~Renderer();

  std::expected<void, std::string> init();
  std::expected<void, std::string> create_window() ;
  std::expected<void, std::string> setup_imgui() ;
  std::expected<void, std::string> run(const std::function<void(float)> &draw_frame) ;

private:
  std::unique_ptr<GLFWwindow, void (*)(GLFWwindow *)> window{nullptr, glfwDestroyWindow};
};

} // namespace lolita

#endif // MY_HEADER_H
