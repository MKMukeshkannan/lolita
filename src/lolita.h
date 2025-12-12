#include <cstddef>
#include <expected>
#include <filesystem>
#include <imgui.h>
#include <memory>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "effect_registry.h"

namespace lolita {

struct Image {
    std::expected<void, std::string>    load_image(std::filesystem::path path);
    std::expected<void, std::string>    observe(EffectsRegistry& registry);
    void                                render_inspector(EffectsRegistry& registry);

    GLuint                              texture_id;
    int                                 height;
    int                                 width;

    std::vector<EffectOffset>           pipeline;
    std::unique_ptr<std::vector<Color>> image;
};

}; // namespace lolita
