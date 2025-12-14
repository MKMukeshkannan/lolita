module;

#include <cstddef>
#include <expected>
#include <functional>
#include <span>
#include <string>
#include <vector>

#include <expected>
#include <imgui.h>
#include <spdlog/spdlog.h>

export module lolita:effects_registry;

export using offset_t = size_t;
template <typename T> using Res = std::expected<T, std::string>;

export struct FloatPair
{
    float prev;
    float curr;
};

namespace lolita {

export struct EffectOffset
{
    size_t type;
    offset_t offset;
};

export struct Color
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

export struct EffectsRegistry
{
    Res<void>
    add_effect(std::string name,
               std::function<void(std::vector<Color> &image, std::span<FloatPair> &p)> effect,
               std::function<void(std::span<FloatPair> &, size_t)> imgui, size_t number_of_params);
    void render_effect_button(std::vector<EffectOffset> &);
    Res<std::span<FloatPair>> get_params(size_t index, size_t offset);
    Res<void> apply_effect(std::vector<Color> &image, EffectOffset effect);

    std::vector<size_t> number_of_params;
    std::vector<FloatPair> parameters;
    std::vector<std::string> name;
    std::vector<std::function<void(std::vector<Color> &image, std::span<FloatPair> &p)>>
        effect_function;
    std::vector<std::function<void(std::span<FloatPair> &p, size_t id)>> controller;
};

Res<void> EffectsRegistry::add_effect(
    std::string name,
    std::function<void(std::vector<Color> &image, std::span<FloatPair> &p)> effect,
    std::function<void(std::span<FloatPair> &, size_t id)> imgui, size_t number_of_params) {

    this->name.push_back(name);
    this->effect_function.push_back(effect);
    this->controller.push_back(imgui);
    this->number_of_params.push_back(number_of_params);

    return {};
};

void EffectsRegistry::render_effect_button(std::vector<EffectOffset> &pipeline) {
    for (size_t i = 0; i < name.size(); ++i) {
        auto effect_name = name[i];
        if (ImGui::Button(effect_name.c_str())) {
            offset_t offset = parameters.size();
            size_t required_param = this->number_of_params[i];
            for (int i = 0; i < required_param; ++i) {
                parameters.push_back({0.0f, 0.0f});
            };

            pipeline.push_back({i, offset});
        }
    };

    return;
};

Res<std::span<FloatPair>> EffectsRegistry::get_params(size_t index, size_t offset) {
    std::span<FloatPair> result = {parameters.data() + offset, number_of_params[index]};
    return result;
};

Res<void> EffectsRegistry::apply_effect(std::vector<Color> &raw_image, EffectOffset effect) {
    auto params = this->get_params(effect.type, effect.offset);
    if (!params)
        return std::unexpected("unable to get params");

    this->effect_function[effect.type](raw_image, *params);
    return {};
};

} // namespace lolita
