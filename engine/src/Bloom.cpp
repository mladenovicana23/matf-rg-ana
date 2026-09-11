#include <engine/graphics/Bloom.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Shader.hpp>

namespace engine::graphics {

void Bloom::initialize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;

    OpenGL::create_hdr_framebuffer(
            m_width,
            m_height,
            m_hdr_fbo,
            m_color_buffers,
            m_depth_rbo
            );

    OpenGL::create_pingpong_framebuffers(
            m_width,
            m_height,
            m_pingpong_fbos,
            m_pingpong_color_buffers
            );

    OpenGL::create_fullscreen_quad(
            m_quad_vao,
            m_quad_vbo
            );
}

void Bloom::destroy() {
    OpenGL::destroy_bloom_resources(
            m_hdr_fbo,
            m_color_buffers,
            m_depth_rbo,
            m_pingpong_fbos,
            m_pingpong_color_buffers,
            m_quad_vao,
            m_quad_vbo
            );

    m_width = 0;
    m_height = 0;

    m_hdr_fbo = 0;
    m_color_buffers[0] = 0;
    m_color_buffers[1] = 0;
    m_depth_rbo = 0;

    m_pingpong_fbos[0] = 0;
    m_pingpong_fbos[1] = 0;
    m_pingpong_color_buffers[0] = 0;
    m_pingpong_color_buffers[1] = 0;

    m_quad_vao = 0;
    m_quad_vbo = 0;
}

void Bloom::resize(uint32_t width, uint32_t height) {
    if (width == m_width && height == m_height) { return; }

    destroy();
    initialize(width, height);
}

void Bloom::begin_scene() const {
    OpenGL::bind_framebuffer(m_hdr_fbo);
    OpenGL::clear_buffers();
}

void Bloom::render(
        resources::Shader *blur_shader,
        resources::Shader *final_shader,
        bool enabled,
        float exposure
        ) const {
    bool horizontal = true;
    bool first_iteration = true;
    const int amount = 10;

    blur_shader->use();
    blur_shader->set_int("image", 0);

    OpenGL::set_depth_test(false);

    for (int i = 0; i < amount; ++i) {
        OpenGL::bind_framebuffer(m_pingpong_fbos[horizontal]);

        blur_shader->set_bool("horizontal", horizontal);

        if (first_iteration) { OpenGL::bind_texture_2d(m_color_buffers[1], 0); } else {
            OpenGL::bind_texture_2d(
                    m_pingpong_color_buffers[!horizontal],
                    0
                    );
        }

        OpenGL::draw_fullscreen_quad(m_quad_vao);

        horizontal = !horizontal;

        if (first_iteration) { first_iteration = false; }
    }

    OpenGL::bind_framebuffer(0);
    OpenGL::clear_buffers();

    final_shader->use();
    final_shader->set_int("scene", 0);
    final_shader->set_int("bloomBlur", 1);
    final_shader->set_bool("bloom", enabled);
    final_shader->set_float("exposure", exposure);

    OpenGL::bind_texture_2d(m_color_buffers[0], 0);
    OpenGL::bind_texture_2d(
            m_pingpong_color_buffers[!horizontal],
            1
            );

    OpenGL::draw_fullscreen_quad(m_quad_vao);

    OpenGL::set_depth_test(true);
}

}