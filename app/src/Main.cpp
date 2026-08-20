#include <engine/core/Engine.hpp>

/**
 * Start here...
 */

class MainController final : public engine::core::Controller {
protected:
    void initialize() override { engine::graphics::OpenGL::enable_depth_testing(); }

    bool loop() override {
        const auto platform = engine::core::Controller::get<engine::platform::PlatformController>();

        if (platform->key(engine::platform::KeyId::KEY_ESCAPE).state() ==
            engine::platform::Key::State::JustPressed) { return false; }

        return true;
    }

    void update() override {}

    void begin_draw() override { engine::graphics::OpenGL::clear_buffers(); }

    void draw() override {}

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
