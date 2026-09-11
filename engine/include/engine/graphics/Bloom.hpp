#ifndef BLOOM_HPP
#define BLOOM_HPP

#include <cstdint>

namespace engine::resources {
class Shader;
}

namespace engine::graphics {

class Bloom {
public:
    Bloom() = default;

    void initialize(uint32_t width, uint32_t height);

    void resize(uint32_t width, uint32_t height);

    void destroy();

    void begin_scene() const;

    void render(
            resources::Shader *blur_shader,
            resources::Shader *final_shader,
            bool enabled,
            float exposure
            ) const;

private:
    uint32_t m_width{0};
    uint32_t m_height{0};

    uint32_t m_hdr_fbo{0};
    uint32_t m_color_buffers[2]{0, 0};
    uint32_t m_depth_rbo{0};

    uint32_t m_pingpong_fbos[2]{0, 0};
    uint32_t m_pingpong_color_buffers[2]{0, 0};

    uint32_t m_quad_vao{0};
    uint32_t m_quad_vbo{0};
};

}

#endif