#include <expected>
#include <filesystem>
#include <memory>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


namespace lolita {

struct Color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    unsigned char alpha;
};

class Image {
public:
    std::expected<void, std::string> load_image(std::filesystem::path path);
    std::expected<void, std::string> observe();


    GLuint                              texture_id;
    int                                 height;
    int                                 width;

    float                               brightness      = 0;
    float                               prev_brightness = 0;

    bool                                grayscale      = 0;
    bool                                prev_grayscale = 0;

    bool                                mutated = false;
private:
    std::shared_ptr<std::vector<Color>> image;
    std::expected<void, std::string>    re_render();
};


}; // namespace lolita
