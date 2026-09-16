#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>
#include <engine/graphics/Bloom.hpp>

class MainController final : public engine::core::Controller {
protected:
    bool m_point_light_enabled = true;
    float m_point_light_x = 0.0f;
    bool m_point_light_warm = true;

    bool m_metronome_event_active = false;
    float m_metronome_event_time = 0.0f;

    engine::graphics::Bloom m_bloom;

    void initialize() override {
        engine::graphics::OpenGL::enable_depth_testing();

        auto platform =
                engine::core::Controller::get<engine::platform::PlatformController>();

        m_bloom.initialize(
                platform->window()->width(),
                platform->window()->height()
                );

        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();

        camera->Position = glm::vec3(0.0f, 2.0f, 15.0f);
        camera->Yaw = -90.0f;
        camera->Pitch = 0.0f;
        camera->rotate_camera(0.0f, 0.0f);
    }

    bool loop() override {
        const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();


        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).state() ==
            engine::platform::Key::State::JustPressed) { return false; }

        if (platform->key(engine::platform::KEY_L).state() ==
            engine::platform::Key::State::JustPressed) { m_point_light_enabled = !m_point_light_enabled; }

        if (platform->key(engine::platform::KEY_M).state() ==
            engine::platform::Key::State::JustPressed) {
            m_metronome_event_active = true;
            m_metronome_event_time = 0.0f;
        }
        if (platform->key(engine::platform::KEY_J).state() ==
            engine::platform::Key::State::Pressed) { m_point_light_x -= 0.05f; }

        if (platform->key(engine::platform::KEY_K).state() ==
            engine::platform::Key::State::Pressed) { m_point_light_x += 0.05f; }

        if (platform->key(engine::platform::KEY_C).state() ==
            engine::platform::Key::State::JustPressed) { m_point_light_warm = !m_point_light_warm; }

        return true;
    }

    void update_camera() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();

        float dt = platform->dt();

        if (platform->key(engine::platform::KEY_W).state() ==
            engine::platform::Key::State::Pressed) { camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt); }

        if (platform->key(engine::platform::KEY_S).state() ==
            engine::platform::Key::State::Pressed) { camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt); }

        if (platform->key(engine::platform::KEY_A).state() ==
            engine::platform::Key::State::Pressed) { camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt); }

        if (platform->key(engine::platform::KEY_D).state() ==
            engine::platform::Key::State::Pressed) { camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt); }
    }

    void update() override {
        update_camera();

        if (m_metronome_event_active) {
            auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
            m_metronome_event_time += platform->dt();

            if (m_metronome_event_time >= 4.0f) {
                m_metronome_event_active = false;
                m_metronome_event_time = 0.0f;
            }
        }
    }

    void begin_draw() override {
        auto platform =
                engine::core::Controller::get<engine::platform::PlatformController>();

        m_bloom.resize(
                platform->window()->width(),
                platform->window()->height()
                );

        m_bloom.begin_scene();
    }

    void draw() override {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        auto shader = resources->shader("basic");
        auto blur_shader = resources->shader("blur");
        auto bloom_final_shader = resources->shader("bloom_final");

        auto classroom = resources->model("classroom");
        auto piano = resources->model("piano");
        auto piano2 = resources->model("piano2");
        auto metronome = resources->model("metronome");
        auto window = resources->model("window");
        auto painting = resources->model("painting");
        auto lamp = resources->model("lamp");
        auto bulb = resources->model("bulb");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        shader->set_bool("emissive", false);
        shader->set_vec3("emissive_color", glm::vec3(0.0f));

        shader->set_vec3(
                "directional_direction",
                glm::vec3(-0.4f, -1.0f, -0.6f)
                );

        shader->set_vec3(
                "directional_color",
                glm::vec3(0.55f, 0.55f, 0.55f)
                );

        shader->set_vec3(
                "point_position",
                glm::vec3(-4.2f + m_point_light_x, 2.005f, -2.5f)
                );

        if (m_point_light_enabled) {
            if (m_point_light_warm) {
                shader->set_vec3(
                        "point_color",
                        glm::vec3(0.8f, 0.7f, 0.55f)
                        );
            } else {
                shader->set_vec3(
                        "point_color",
                        glm::vec3(0.45f, 0.65f, 1.0f)
                        );
            }
        } else {
            shader->set_vec3(
                    "point_color",
                    glm::vec3(0.0f)
                    );
        }

        shader->set_vec3("fallback_color", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->set_vec3("texture_tint", glm::vec3(0.75f, 0.45f, 0.25f));

        shader->set_mat4("model", glm::mat4(1.0f));
        classroom->draw(shader);

        shader->set_vec3("texture_tint", glm::vec3(1.0f, 1.0f, 1.0f));

        glm::mat4 piano_model = glm::mat4(1.0f);
        piano_model = glm::translate(piano_model, glm::vec3(-2.0f, 1.6f, -1.5f));
        piano_model = glm::scale(piano_model, glm::vec3(0.05f));

        shader->set_mat4("model", piano_model);
        piano->draw(shader);

        glm::mat4 piano2_model = glm::mat4(1.0f);
        piano2_model = glm::translate(piano2_model, glm::vec3(2.5f, 0.7f, -2.0f));
        piano2_model = glm::rotate(
                piano2_model,
                glm::radians(180.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
                );

        piano2_model = glm::scale(piano2_model, glm::vec3(0.07f));
        shader->set_mat4("model", piano2_model);
        piano2->draw(shader);

        glm::vec3 metronome_position = glm::vec3(1.8f, 1.8f, -2.0f);

        if (m_metronome_event_active) { if (m_metronome_event_time >= 1.0f && m_metronome_event_time < 3.0f) { metronome_position.x -= 0.3f; } else if (m_metronome_event_time >= 3.0f) { metronome_position.x += 0.3f; } }

        glm::mat4 metronome_model = glm::mat4(1.0f);
        metronome_model = glm::translate(
                metronome_model,
                metronome_position
                );

        metronome_model = glm::scale(metronome_model, glm::vec3(1.2f));
        shader->set_vec3("fallback_color", glm::vec3(0.08f, 0.06f, 0.04f));
        shader->set_vec3("texture_tint", glm::vec3(1.0f));
        shader->set_mat4("model", metronome_model);
        metronome->draw(shader);

        glm::mat4 window_model = glm::mat4(1.0f);
        window_model = glm::translate(window_model, glm::vec3(5.0f, 2.0f, 0.3f));
        window_model = glm::rotate(
                window_model,
                glm::radians(-90.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
                );
        window_model = glm::scale(window_model, glm::vec3(0.008f));

        shader->set_vec3("fallback_color", glm::vec3(0.35f, 0.18f, 0.08f));
        shader->set_mat4("model", window_model);
        window->draw(shader);


        glm::mat4 painting_model = glm::mat4(1.0f);
        painting_model = glm::translate(painting_model, glm::vec3(0.0f, 2.2f, -3.95f));
        painting_model = glm::rotate(
                painting_model,
                glm::radians(180.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
                );
        painting_model = glm::scale(painting_model, glm::vec3(0.009f));

        shader->set_vec3("texture_tint", glm::vec3(1.0f));
        shader->set_vec3("fallback_color", glm::vec3(1.0f));
        shader->set_mat4("model", painting_model);
        painting->draw(shader);

        glm::mat4 lamp_model = glm::mat4(1.0f);
        lamp_model = glm::translate(lamp_model, glm::vec3(-4.2f, 0.0f, -2.5f));
        lamp_model = glm::scale(lamp_model, glm::vec3(0.35f));

        shader->set_bool("emissive", false);
        shader->set_vec3("emissive_color", glm::vec3(0.0f));

        shader->set_vec3("texture_tint", glm::vec3(1.0f));
        shader->set_vec3("fallback_color", glm::vec3(1.0f));
        shader->set_mat4("model", lamp_model);
        lamp->draw(shader);

        glm::mat4 bulb_model = glm::mat4(1.0f);

        bulb_model = glm::translate(bulb_model, glm::vec3(-4.2f, 2.005f, -2.5f));

        bulb_model = glm::scale(bulb_model, glm::vec3(0.15f));

        shader->set_bool("emissive", m_point_light_enabled);

        if (m_point_light_enabled) {
            shader->set_vec3(
                    "emissive_color",
                    m_point_light_warm
                        ? glm::vec3(15.0f, 12.0f, 7.0f)
                        : glm::vec3(7.0f, 11.0f, 15.0f)
                    );
        } else {
            shader->set_vec3(
                    "emissive_color",
                    glm::vec3(0.0f)
                    );
        }

        shader->set_vec3(
                "fallback_color",
                glm::vec3(1.0f)
                );

        shader->set_vec3(
                "texture_tint",
                glm::vec3(1.0f)
                );

        shader->set_mat4("model", bulb_model);

        bulb->draw(shader);

        m_bloom.render(
                blur_shader,
                bloom_final_shader,
                true,
                1.0f
                );
    }

    void terminate() override { m_bloom.destroy(); }

    void end_draw() override { engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers(); }
};

class MainApp final : public engine::core::App {
protected:
    void app_setup() override {
        auto main_controller = register_controller<MainController>();
        main_controller->after(
                engine::core::Controller::get<engine::core::EngineControllersEnd>()
                );
    }
};

int main(int argc, char **argv) { return std::make_unique<MainApp>()->run(argc, argv); }