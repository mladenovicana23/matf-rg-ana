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
            float exposure) const;

private:
    struct HdrFramebuffer {
        uint32_t framebuffer{0};
        uint32_t color_buffers[2]{0, 0};
        uint32_t depth_renderbuffer{0};
    };

    struct PingpongFramebuffers {
        uint32_t framebuffers[2]{0, 0};
        uint32_t color_buffers[2]{0, 0};
    };

    struct FullscreenQuad {
        uint32_t vao{0};
        uint32_t vbo{0};
    };

    HdrFramebuffer create_hdr_framebuffer() const;

    PingpongFramebuffers create_pingpong_framebuffers() const;

    FullscreenQuad create_fullscreen_quad() const;

    uint32_t m_width{0};
    uint32_t m_height{0};

    HdrFramebuffer m_hdr;
    PingpongFramebuffers m_pingpong;
    FullscreenQuad m_quad;
};

}// namespace engine::graphics

#endif