#pragma once

#include <cstddef>
#include <expected>
#include <functional>
#include <span>
#include <string>
#include <vector>

using offset_t = size_t;
struct FloatPair {
    float prev;
    float curr;
};

namespace lolita {

struct EffectOffset {
    size_t type;
    offset_t offset;
};

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};


struct EffectsRegistry {
    std::expected<void, std::string>
    add_effect(std::string name,
                std::function<void(std::vector<Color> &image, std::span<FloatPair> &p)> effect,
                std::function<void(std::span<FloatPair> &, size_t)> imgui,
                size_t number_of_params);

    void 
    render_effect_button(std::vector<EffectOffset> &);

    std::expected<std::span<FloatPair>, std::string> 
    get_params(size_t index, size_t offset);

    std::expected<void, std::string>
    apply_effect(std::vector<Color> &image, EffectOffset effect);

    std::vector<size_t>                                                  number_of_params;
    std::vector<FloatPair>                                               parameters;
    std::vector<std::string>                                             name;
    std::vector<std::function<void(std::vector<Color> &image, std::span<FloatPair> &p)>> effect_function;
    std::vector<std::function<void(std::span<FloatPair> &p, size_t id)>> controller;
};

} // namespace lolita
