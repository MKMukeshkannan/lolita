
#include "renderer.h"

#include <expected>
#include <spdlog/spdlog.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

lolita::Renderer::~Renderer() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwTerminate();
};

std::expected<void, std::string> lolita::Renderer::init() {
  if (!glfwInit())
    return std::unexpected("unable to initilize glfw");

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  return {};
};

std::expected<void, std::string> lolita::Renderer::create_window() {
  window.reset(glfwCreateWindow(1280, 720, "Hello", nullptr, nullptr));
  if (!window) {
    glfwTerminate();
    return std::unexpected("unable to create window");
  }
  glfwMakeContextCurrent(window.get());
  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK) {
    return std::unexpected("unable to initialise glew");
  };

  return {};
};

std::expected<void, std::string> lolita::Renderer::setup_imgui() {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;

  ImGui::StyleColorsDark();

  ImGui_ImplGlfw_InitForOpenGL(window.get(), true);
  ImGui_ImplOpenGL3_Init("#version 330");

  return {};
};

std::expected<void, std::string>
lolita::Renderer::run(const std::function<void(float)> &draw_frame) {

  float last_time = (float)glfwGetTime();
  while (!glfwWindowShouldClose(window.get())) {

    float current_time = (float)glfwGetTime();
    float delta_time = current_time - last_time;
    last_time = current_time;

    glfwPollEvents();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    draw_frame(delta_time);

    // Demo window

    // Rendering
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window.get(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window.get());
  };

  return {};
};

bool lolita::LoadTextureFromMemory(const void *data, size_t data_size,
                                   GLuint *out_texture, int *out_width,
                                   int *out_height) {
  int image_width = 0;
  int image_height = 0;
  unsigned char *image_data =
      stbi_load_from_memory((const unsigned char *)data, (int)data_size,
                            &image_width, &image_height, NULL, 4);
  if (image_data == NULL)
    return false;

  GLuint image_texture;
  glGenTextures(1, &image_texture);
  glBindTexture(GL_TEXTURE_2D, image_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, image_data);
  stbi_image_free(image_data);

  *out_texture = image_texture;
  *out_width = image_width;
  *out_height = image_height;

  return true;
};

bool lolita::LoadTextureFromFile(const char *file_name, GLuint *out_texture,
                                 int *out_width, int *out_height) {
  FILE *f = fopen(file_name, "rb");
  if (f == NULL)
    return false;
  fseek(f, 0, SEEK_END);
  size_t file_size = (size_t)ftell(f);
  if (file_size == -1)
    return false;
  fseek(f, 0, SEEK_SET);
  void *file_data = IM_ALLOC(file_size);
  fread(file_data, 1, file_size, f);
  fclose(f);
  bool ret = LoadTextureFromMemory(file_data, file_size, out_texture, out_width,
                                   out_height);
  IM_FREE(file_data);
  return ret;
};
