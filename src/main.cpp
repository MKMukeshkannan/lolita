#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <expected>
#include <print>
#include <span>
#include <spdlog/logger.h>
#include <spdlog/spdlog.h>
#include <string>

#include "imgui.h"

#include "lolita.h"
#include "renderer.h"
#include "src/effect_registry.h"

void brightness(std::vector<lolita::Color>& temp_image, std::span<FloatPair> param) { 
        const int16_t t_brightness = static_cast<char>(param[0].curr);

        spdlog::info("brig {}", t_brightness);
        spdlog::info(" -> {}", param[0].curr);

        for (int i = 0; i < 512 * 512; ++i) {
            (temp_image[i]).red   = std::clamp(temp_image[i].red + t_brightness, 0, 255);
            (temp_image[i]).green = std::clamp(temp_image[i].green + t_brightness, 0, 255);
            (temp_image[i]).blue  = std::clamp(temp_image[i].blue + t_brightness, 0, 255);
        };
};
void brightness_controller(std::span<FloatPair> param, size_t id) { 

    ImGui::PushID(id);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    {
        ImGui::BeginChild("BRIGHTNESS", ImVec2(-FLT_MIN, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
        {
            ImGui::Button("X");
            ImGui::SameLine();
            ImGui::SeparatorText("brightness");
            ImGui::SliderFloat("brightness", &param[0].curr, -255.0f, 255.0f);
        }
        ImGui::EndChild();
    }
    ImGui::PopStyleColor();
    ImGui::PopID();

};

void half_tone(std::vector<lolita::Color> &temp_image, std::span<FloatPair> param) {
        const int16_t t_red = static_cast<char>(param[0].curr);
        const int16_t t_green = static_cast<char>(param[1].curr);
        const int16_t t_blue = static_cast<char>(param[2].curr);

        spdlog::info("brig {}", t_red);
        spdlog::info(" -> {}", param[0].curr);

        for (int i = 0; i < 512 * 512; ++i) {
            (temp_image[i]).red   = std::clamp(temp_image[i].red + t_red, 0, 255);
            (temp_image[i]).green = std::clamp(temp_image[i].green + t_green, 0, 255);
            (temp_image[i]).blue  = std::clamp(temp_image[i].blue + t_blue, 0, 255);
        };
};
void half_tone_controller(std::span<FloatPair> param, size_t id) { 

    ImGui::PushID(id);
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_FrameBg));
    {
        ImGui::BeginChild("HALFTONE", ImVec2(-FLT_MIN, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_AutoResizeY);
        {
            ImGui::Button("X");
            ImGui::SameLine();
            ImGui::SeparatorText("Halftone");
            ImGui::SliderFloat("red", &param[0].curr, -255.0f, 255.0f);
            ImGui::SliderFloat("green", &param[1].curr, -255.0f, 255.0f);
            ImGui::SliderFloat("blue", &param[2].curr, -255.0f, 255.0f);
        }
        ImGui::EndChild();
    }
    ImGui::PopStyleColor();
    ImGui::PopID();

};

int main() 
{
  std::expected<void, std::string> res;


  // EFFECT PANEL CONFIGURATION
  lolita::EffectsRegistry registry;
  res = registry.add_effect("brightness", brightness, brightness_controller, 1);
  res = registry.add_effect("half_tone",  half_tone,  half_tone_controller,  3);


  // RENDER SETUP
  lolita::Renderer renderer;
  if (auto res = renderer.init(); !res)
    return spdlog::error(res.error()), -1;

  if (auto res = renderer.create_window(); !res)
    return spdlog::error(res.error()), -1;

  if (auto res = renderer.setup_imgui(); !res)
    return spdlog::error(res.error()), -1;

  spdlog::set_level(spdlog::level::debug);


  // IMAGE SETUP
  lolita::Image image;
  if (auto res = image.load_image("samples/megan.png"); !res)
    return spdlog::error(res.error()), -1;

  spdlog::info("{} {}", image.height, image.width);

  res = renderer.run([&](float dt) {
    if (auto res = image.observe(registry); !res)
       return spdlog::error(res.error());

    ImGuiIO &io          = ImGui::GetIO();
    ImVec2 viewport_pos  = ImGui::GetMainViewport()->Pos;
    ImVec2 viewport_size = ImGui::GetMainViewport()->Size;


    float right_width = 300.0f;
    {
      ImGui::SetNextWindowPos(ImVec2( viewport_pos.x + viewport_size.x - right_width, viewport_pos.y));
      ImGui::SetNextWindowSize(ImVec2(right_width, 500));

      ImGui::Begin("Inspector", nullptr, renderer.window_flag);
      {
        image.render_inspector(registry);
      }
      ImGui::End();
    }

    // ImGui::ShowDemoWindow();
    // PROPERTIES
    {
      ImGui::SetNextWindowPos(ImVec2( viewport_pos.x + viewport_size.x - right_width, viewport_pos.y + 500));
      ImGui::SetNextWindowSize(ImVec2(right_width, viewport_size.y - 500));

      ImGui::Begin("Fx Panel", nullptr, renderer.window_flag);
      {
        registry.render_effect_button(image.pipeline);
      }
      ImGui::End();
    }

    // IMAGE
    {
      ImGui::SetNextWindowPos(viewport_pos);
      ImGui::SetNextWindowSize( ImVec2(viewport_size.x - right_width, viewport_size.y));

      ImGui::Begin("Image Editor", nullptr, renderer.window_flag);
      ImGui::Image((ImTextureID)(intptr_t)image.texture_id, ImVec2(image.width, image.height));
      ImGui::End();
    }

  });

  return 0;
};
