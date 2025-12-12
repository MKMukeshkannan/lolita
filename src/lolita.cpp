#include "lolita.h"

#include <cstddef>
#include <expected>
#include <fstream>
#include <memory>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <string>
#include <vector>

#include "src/effect_registry.h"
#include "stb_image.h"

std::expected<void, std::string>
lolita::Image::load_image(std::filesystem::path path) {
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if (!file.is_open())
    return std::unexpected("cannot open the file");

  std::size_t filesize = file.tellg();
  file.seekg(0, std::ios::beg);

  auto buf = std::vector<unsigned char>(filesize);
  file.read(reinterpret_cast<char *>(buf.data()), filesize);
  file.close();

  auto *image_data = stbi_load_from_memory((const unsigned char *)buf.data(), (int)filesize, &width, &height, NULL, 4);

  if (image_data == NULL)
    return std::unexpected("cannot load image into stbi memory");

  const auto number_of_color = height * width;

  image = std::make_unique<std::vector<Color>>(number_of_color);
  for (size_t i = 0; i < height * width; ++i) {
    size_t idx = i * 4;

    Color c{};
    (*image)[i].red = image_data[idx + 0];
    (*image)[i].green = image_data[idx + 1];
    (*image)[i].blue = image_data[idx + 2];
    (*image)[i].alpha = image_data[idx + 3];
  };
  stbi_image_free(image_data);

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image->data());


  return {};
};


std::expected<void, std::string> lolita::Image::observe(lolita::EffectsRegistry& registry) {

    bool is_mutated = false;
    std::vector<Color> temp_image;
    for (const auto &[type, offset]: pipeline) {
        if (is_mutated) break;

        auto params = registry.get_params(type, offset);
        if (!params) {
            spdlog::trace("lolita::observer, couldn't get params | {}", params.error());
            return std::unexpected(params.error());
        };

        for (auto &param: *params) {
            if (param.curr != param.prev) {
                param.prev = param.curr;
                is_mutated = true;
                temp_image = std::vector<Color>(*image);
            };
        };
    };

    if (!is_mutated) return {};

    for (const auto &[type, offset]: pipeline) {
        auto params = registry.get_params(type, offset);
        if (!params) {
            spdlog::trace("lolita::observer, couldn't get params | {}", params.error());
            return std::unexpected(params.error());
        };

        auto res = registry.apply_effect(temp_image, {type, offset});
        if (!res) return res;
    };

    if (is_mutated) {
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,  GL_RGBA, GL_UNSIGNED_BYTE, temp_image.data());
    };

//    bool is_mutated = false;
//
//    if (is_mutated) {
//        const char t_brightness = static_cast<char>(brightness);
//        auto temp_image = std::vector<Color>(*image);
//
//        for (int i = 0; i < height * width; ++i) {
//            (temp_image[i]).red   = std::clamp((*image)[i].red + t_brightness, 0, 255);
//            (temp_image[i]).green = std::clamp((*image)[i].green + t_brightness, 0, 255);
//            (temp_image[i]).blue  = std::clamp((*image)[i].blue + t_brightness, 0, 255);
//        };
//
//        if (grayscale) {
//            for (int i = 0; i < height * width; ++i) {
//                auto avg = (temp_image[i]).red + (temp_image[i]).green + (temp_image[i]).blue;
//                (temp_image[i]).red   = std::clamp(avg, 0, 255); 
//                (temp_image[i]).green = std::clamp(avg, 0, 255);
//                (temp_image[i]).blue  = std::clamp(avg, 0, 255);
//            };
//        };
//        
//        if (grid_size != 0) {
//            for (int i = 0; i < height * width; ++i) {
//                int row = i / width, col = i % width;
//
//                if (row % grid_size) {
//                    temp_image[i].alpha = 0;
//                };
//                if (col % grid_size) {
//                    temp_image[i].alpha = 0;
//                };
//            };
//        };
//
//
//        glBindTexture(GL_TEXTURE_2D, texture_id);
//        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height,  GL_RGBA, GL_UNSIGNED_BYTE, temp_image.data());
//
//        mutated = false;
//    };

    return {};
};


void
lolita::Image::render_inspector(lolita::EffectsRegistry& effects_registry) {
    for (const auto &[type, offset]: pipeline) {
        auto param = effects_registry.get_params(type, offset);
        if (!param) 
        {
            return;
        };

        // calling the function 
        effects_registry.controller[type](param.value(), type+offset);
    };
};



/*
 * [1, 2, 3, 4]
 * struct brightness {
 *      size   = 4;
 *      offset = 0
 * };
 * EFFECT REGISTRY:
 * functions             :
 * parameters(vec<float>):  [BBBB_ _ _ _ _ _ _ _ _ ]
 *
 * funcions(vec<fun>)    :  [ bright line riple    ]
 * number_of_parm(int)   :  [     01   b4    03    ]
 *
 */
