#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

class MainController final : public engine::core::Controller {
protected:
    bool point_light_enabled = true;
    float point_light_x = 0.0f;
    bool point_light_warm = true;

    bool metronome_event_active = false;
    float metronome_event_time = 0.0f;

    void initialize() override {
        engine::graphics::OpenGL::enable_depth_testing();

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
            engine::platform::Key::State::JustPressed) { point_light_enabled = !point_light_enabled; }

        if (platform->key(engine::platform::KEY_M).state() ==
            engine::platform::Key::State::JustPressed) {
            metronome_event_active = true;
            metronome_event_time = 0.0f;
        }
        if (platform->key(engine::platform::KEY_J).state() ==
            engine::platform::Key::State::Pressed) { point_light_x -= 0.05f; }

        if (platform->key(engine::platform::KEY_K).state() ==
            engine::platform::Key::State::Pressed) { point_light_x += 0.05f; }

        if (platform->key(engine::platform::KEY_C).state() ==
            engine::platform::Key::State::JustPressed) { point_light_warm = !point_light_warm; }

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

        if (metronome_event_active) {
            auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
            metronome_event_time += platform->dt();

            if (metronome_event_time >= 4.0f) {
                metronome_event_active = false;
                metronome_event_time = 0.0f;
            }
        }
    }

    void begin_draw() override { engine::graphics::OpenGL::clear_buffers(); }

    void draw() override {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        auto shader = resources->shader("basic");
        auto classroom = resources->model("classroom");
        auto piano = resources->model("piano");
        auto piano2 = resources->model("piano2");
        auto metronome = resources->model("metronome");
        auto window = resources->model("window");
        auto painting = resources->model("painting");
        auto lamp = resources->model("lamp");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

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
                glm::vec3(point_light_x, 3.0f, 0.0f)
                );

        if (point_light_enabled) {
            if (point_light_warm) {
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

        if (metronome_event_active) { if (metronome_event_time >= 1.0f && metronome_event_time < 3.0f) { metronome_position.x -= 0.3f; } else if (metronome_event_time >= 3.0f) { metronome_position.x += 0.3f; } }

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

        glm::mat4 windowModel = glm::mat4(1.0f);
        windowModel = glm::translate(windowModel, glm::vec3(5.0f, 2.0f, 0.3f));
        windowModel = glm::rotate(
                windowModel,
                glm::radians(-90.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
                );
        windowModel = glm::scale(windowModel, glm::vec3(0.008f));

        shader->set_vec3("fallback_color", glm::vec3(0.35f, 0.18f, 0.08f));
        shader->set_mat4("model", windowModel);
        window->draw(shader);


        glm::mat4 paintingModel = glm::mat4(1.0f);
        paintingModel = glm::translate(paintingModel, glm::vec3(0.0f, 2.2f, -3.95f));
        paintingModel = glm::rotate(
                paintingModel,
                glm::radians(180.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
                );
        paintingModel = glm::scale(paintingModel, glm::vec3(0.009f));

        shader->set_vec3("texture_tint", glm::vec3(1.0f));
        shader->set_vec3("fallback_color", glm::vec3(1.0f));
        shader->set_mat4("model", paintingModel);
        painting->draw(shader);

        glm::mat4 lampModel = glm::mat4(1.0f);
        lampModel = glm::translate(lampModel, glm::vec3(-4.2f, 0.0f, -2.5f));
        lampModel = glm::scale(lampModel, glm::vec3(0.35f));

        shader->set_vec3("texture_tint", glm::vec3(1.0f));
        shader->set_vec3("fallback_color", glm::vec3(1.0f));
        shader->set_mat4("model", lampModel);
        lamp->draw(shader);
    }

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