#include "effect_registry.h"
#include <expected>
#include <imgui.h>
#include <spdlog/spdlog.h>


std::expected<void, std::string> 
lolita::EffectsRegistry::add_effect(
        std::string name,
        std::function<void(std::vector<Color> &image, std::span<FloatPair> &p)> effect,
        std::function<void(std::span<FloatPair>&, size_t id)> imgui, 
        size_t number_of_params) 
{
    this->name.push_back(name);
    this->effect_function.push_back(effect);
    this->controller.push_back(imgui);
    this->number_of_params.push_back(number_of_params);

    return {};
};

void 
lolita::EffectsRegistry::render_effect_button(std::vector<lolita::EffectOffset>& pipeline) {
    for (size_t i = 0; i < name.size(); ++i) 
    {
        auto effect_name = name[i];
        if (ImGui::Button(effect_name.c_str())) 
        { 

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

std::expected<std::span<FloatPair>, std::string> 
lolita::EffectsRegistry::get_params(size_t index, size_t offset) 
{
    std::span<FloatPair> result = { parameters.data() + offset, number_of_params[index] };
    return result;
};

std::expected<void, std::string>
lolita::EffectsRegistry::apply_effect(std::vector<lolita::Color>& raw_image, lolita::EffectOffset effect)
{
    auto params = this->get_params(effect.type, effect.offset);
    if (!params) 
        return std::unexpected("unable to get params");

    this->effect_function[effect.type](raw_image, *params);
    return {};
};
