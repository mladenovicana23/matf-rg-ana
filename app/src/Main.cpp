#include <engine/core/Engine.hpp>
#include <engine/graphics/GraphicsController.hpp>

/**
 * Start here...
 */

class MainController final : public engine::core::Controller {
protected:
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

        return true;
    }

    void update_camera() {
        auto platform = engine::core::Controller::get<engine::platform::PlatformController>();
        auto camera = engine::core::Controller::get<engine::graphics::GraphicsController>()->camera();

        float dt = platform->dt();

        if (platform->key(engine::platform::KEY_W).state() == engine::platform::Key::State::Pressed) { camera->move_camera(engine::graphics::Camera::Movement::FORWARD, dt); }

        if (platform->key(engine::platform::KEY_S).state() == engine::platform::Key::State::Pressed) { camera->move_camera(engine::graphics::Camera::Movement::BACKWARD, dt); }

        if (platform->key(engine::platform::KEY_A).state() == engine::platform::Key::State::Pressed) { camera->move_camera(engine::graphics::Camera::Movement::LEFT, dt); }

        if (platform->key(engine::platform::KEY_D).state() == engine::platform::Key::State::Pressed) { camera->move_camera(engine::graphics::Camera::Movement::RIGHT, dt); }


    }

    void update() override { update_camera(); }

    void begin_draw() override { engine::graphics::OpenGL::clear_buffers(); }

    void draw() override {
        auto graphics = engine::core::Controller::get<engine::graphics::GraphicsController>();
        auto resources = engine::core::Controller::get<engine::resources::ResourcesController>();

        auto shader = resources->shader("basic");
        auto classroom = resources->model("classroom");
        auto piano = resources->model("piano");
        auto piano2 = resources->model("piano2");
        auto metronome = resources->model("metronome");

        shader->use();
        shader->set_mat4("projection", graphics->projection_matrix());
        shader->set_mat4("view", graphics->camera()->view_matrix());

        shader->set_mat4("model", glm::mat4(1.0f));
        classroom->draw(shader);

        glm::mat4 piano_model = glm::mat4(1.0f);
        piano_model = glm::translate(piano_model, glm::vec3(-2.0f, 1.6f, -1.5f));
        piano_model = glm::scale(piano_model, glm::vec3(0.05f));
        shader->set_mat4("model", piano_model);
        piano->draw(shader);

        glm::mat4 piano2_model = glm::mat4(1.0f);

        piano2_model = glm::translate(piano2_model, glm::vec3(2.5, 0.7f, -2.0f));

        piano2_model = glm::rotate(piano2_model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        piano2_model = glm::scale(piano2_model, glm::vec3(0.07f));

        shader->set_mat4("model", piano2_model);
        piano2->draw(shader);


        glm::mat4 metronome_model = glm::mat4(1.0f);
        metronome_model = glm::translate(metronome_model, glm::vec3(1.8f, 1.8f, -2.0f));
        metronome_model = glm::scale(metronome_model, glm::vec3(1.2f));

        shader->set_mat4("model", metronome_model);
        metronome->draw(shader);
    }

    void end_draw() override { engine::core::Controller::get<engine::platform::PlatformController>()->swap_buffers(); }
};

class MainApp final : public engine::core::App {
protected:
    void app_setup() override {
        auto main_controller = register_controller<MainController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    }
};

int main(int argc, char **argv) { return std::make_unique<MainApp>()->run(argc, argv); }