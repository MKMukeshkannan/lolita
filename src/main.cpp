#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include "imgui.h"

#include "lolita.h"
#include "renderer.h"

int main() {

  lolita::Renderer renderer;
  lolita::Image image;

  if (auto res = renderer.init(); !res)
    return spdlog::error(res.error()), -1;

  if (auto res = renderer.create_window(); !res)
    return spdlog::error(res.error()), -1;

  if (auto res = renderer.setup_imgui(); !res)
    return spdlog::error(res.error()), -1;

  spdlog::set_level(spdlog::level::debug);

  // RENDERER

  if (auto res = image.load_image("samples/megan.png"); !res)
    return spdlog::error(res.error()), -1;

  auto res = renderer.run([&](float dt) {
    if (auto res = image.observe(); !res)
      return spdlog::error(res.error());

    ImGui::Begin("Example Window");
    ImGui::SliderFloat("Value", &image.brightness, -255.0f, 255.0f);
    ImGui::Text("Value: %.3f", image.brightness);
    ImGui::Checkbox("GRAYSCALE", &image.grayscale);
    ImGui::End();

    ImGui::Begin("OpenGL Texture Text");
    ImGui::Image((ImTextureID)(intptr_t)image.texture_id,
                 ImVec2(image.width, image.height));
    ImGui::End();
  });

  return 0;
};
