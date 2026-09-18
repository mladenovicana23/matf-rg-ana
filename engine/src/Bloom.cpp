// clang-format off
#include <glad/glad.h>
// clang-format on

#include <engine/graphics/Bloom.hpp>
#include <engine/graphics/OpenGL.hpp>
#include <engine/resources/Shader.hpp>
#include <engine/util/Errors.hpp>

namespace engine::graphics {

Bloom::HdrFramebuffer Bloom::create_hdr_framebuffer() const {
    HdrFramebuffer result{};

    CHECKED_GL_CALL(glGenFramebuffers, 1, &result.framebuffer);
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, result.framebuffer);

    CHECKED_GL_CALL(glGenTextures, 2, result.color_buffers);

    for (uint32_t i = 0; i < 2; i++) {
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, result.color_buffers[i]);

        CHECKED_GL_CALL(
                glTexImage2D,
                GL_TEXTURE_2D,
                0,
                GL_RGBA16F,
                m_width,
                m_height,
                0,
                GL_RGBA,
                GL_FLOAT,
                nullptr
                );

        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        CHECKED_GL_CALL(
                glFramebufferTexture2D,
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + i,
                GL_TEXTURE_2D,
                result.color_buffers[i],
                0
                );
    }

    CHECKED_GL_CALL(glGenRenderbuffers, 1, &result.depth_renderbuffer);
    CHECKED_GL_CALL(glBindRenderbuffer, GL_RENDERBUFFER, result.depth_renderbuffer);

    CHECKED_GL_CALL(
            glRenderbufferStorage,
            GL_RENDERBUFFER,
            GL_DEPTH_COMPONENT,
            m_width,
            m_height
            );

    CHECKED_GL_CALL(
            glFramebufferRenderbuffer,
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_RENDERBUFFER,
            result.depth_renderbuffer
            );

    uint32_t attachments[2] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1
    };

    CHECKED_GL_CALL(glDrawBuffers, 2, attachments);

    if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw util::EngineError(
                util::EngineError::Type::OpenGLError,
                "HDR framebuffer is not complete"
                );
    }

    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

    return result;
}

Bloom::PingpongFramebuffers Bloom::create_pingpong_framebuffers() const {
    PingpongFramebuffers result{};

    CHECKED_GL_CALL(glGenFramebuffers, 2, result.framebuffers);
    CHECKED_GL_CALL(glGenTextures, 2, result.color_buffers);

    for (uint32_t i = 0; i < 2; i++) {
        CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, result.framebuffers[i]);
        CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, result.color_buffers[i]);

        CHECKED_GL_CALL(
                glTexImage2D,
                GL_TEXTURE_2D,
                0,
                GL_RGBA16F,
                m_width,
                m_height,
                0,
                GL_RGBA,
                GL_FLOAT,
                nullptr
                );

        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        CHECKED_GL_CALL(glTexParameteri, GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        CHECKED_GL_CALL(
                glFramebufferTexture2D,
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                result.color_buffers[i],
                0
                );

        if (CHECKED_GL_CALL(glCheckFramebufferStatus, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            throw util::EngineError(
                    util::EngineError::Type::OpenGLError,
                    "Ping-pong framebuffer is not complete"
                    );
        }
    }

    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);

    return result;
}

Bloom::FullscreenQuad Bloom::create_fullscreen_quad() const {
    FullscreenQuad result{};

    float quad_vertices[] = {
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };

    CHECKED_GL_CALL(glGenVertexArrays, 1, &result.vao);
    CHECKED_GL_CALL(glGenBuffers, 1, &result.vbo);

    CHECKED_GL_CALL(glBindVertexArray, result.vao);
    CHECKED_GL_CALL(glBindBuffer, GL_ARRAY_BUFFER, result.vbo);

    CHECKED_GL_CALL(
            glBufferData,
            GL_ARRAY_BUFFER,
            sizeof(quad_vertices),
            quad_vertices,
            GL_STATIC_DRAW
            );

    CHECKED_GL_CALL(glEnableVertexAttribArray, 0);
    CHECKED_GL_CALL(
            glVertexAttribPointer,
            0,
            2,
            GL_FLOAT,
            GL_FALSE,
            4 * sizeof(float),
            (void *) 0
            );

    CHECKED_GL_CALL(glEnableVertexAttribArray, 1);
    CHECKED_GL_CALL(
            glVertexAttribPointer,
            1,
            2,
            GL_FLOAT,
            GL_FALSE,
            4 * sizeof(float),
            (void *) (2 * sizeof(float))
            );

    CHECKED_GL_CALL(glBindVertexArray, 0);

    return result;
}

void Bloom::initialize(uint32_t width, uint32_t height) {
    m_width = width;
    m_height = height;

    m_hdr = create_hdr_framebuffer();
    m_pingpong = create_pingpong_framebuffers();
    m_quad = create_fullscreen_quad();
}

void Bloom::destroy() {
    CHECKED_GL_CALL(glDeleteFramebuffers, 1, &m_hdr.framebuffer);
    CHECKED_GL_CALL(glDeleteTextures, 2, m_hdr.color_buffers);
    CHECKED_GL_CALL(glDeleteRenderbuffers, 1, &m_hdr.depth_renderbuffer);

    CHECKED_GL_CALL(glDeleteFramebuffers, 2, m_pingpong.framebuffers);
    CHECKED_GL_CALL(glDeleteTextures, 2, m_pingpong.color_buffers);

    CHECKED_GL_CALL(glDeleteVertexArrays, 1, &m_quad.vao);
    CHECKED_GL_CALL(glDeleteBuffers, 1, &m_quad.vbo);

    m_width = 0;
    m_height = 0;

    m_hdr = {};
    m_pingpong = {};
    m_quad = {};
}

void Bloom::resize(uint32_t width, uint32_t height) {
    if (width == m_width && height == m_height) { return; }

    destroy();
    initialize(width, height);
}

void Bloom::begin_scene() const {
    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, m_hdr.framebuffer);
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
    const int AMOUNT = 10;

    blur_shader->use();
    blur_shader->set_int("image", 0);

    OpenGL::disable_depth_testing();

    for (int i = 0; i < AMOUNT; ++i) {
        CHECKED_GL_CALL(
                glBindFramebuffer,
                GL_FRAMEBUFFER,
                m_pingpong.framebuffers[horizontal]
                );

        blur_shader->set_bool("horizontal", horizontal);

        CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);

        if (first_iteration) { CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_hdr.color_buffers[1]); } else {
            CHECKED_GL_CALL(
                    glBindTexture,
                    GL_TEXTURE_2D,
                    m_pingpong.color_buffers[!horizontal]
                    );
        }

        CHECKED_GL_CALL(glBindVertexArray, m_quad.vao);
        CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 6);
        CHECKED_GL_CALL(glBindVertexArray, 0);

        horizontal = !horizontal;

        if (first_iteration) { first_iteration = false; }
    }

    CHECKED_GL_CALL(glBindFramebuffer, GL_FRAMEBUFFER, 0);
    OpenGL::clear_buffers();

    final_shader->use();
    final_shader->set_int("scene", 0);
    final_shader->set_int("bloomBlur", 1);
    final_shader->set_bool("bloom", enabled);
    final_shader->set_float("exposure", exposure);

    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE0);
    CHECKED_GL_CALL(glBindTexture, GL_TEXTURE_2D, m_hdr.color_buffers[0]);

    CHECKED_GL_CALL(glActiveTexture, GL_TEXTURE1);
    CHECKED_GL_CALL(
            glBindTexture,
            GL_TEXTURE_2D,
            m_pingpong.color_buffers[!horizontal]
            );

    CHECKED_GL_CALL(glBindVertexArray, m_quad.vao);
    CHECKED_GL_CALL(glDrawArrays, GL_TRIANGLES, 0, 6);
    CHECKED_GL_CALL(glBindVertexArray, 0);

    OpenGL::enable_depth_testing();
}

}