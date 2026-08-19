#include <engine/core/Engine.hpp>

/**
 * Start here...
 */

class MainController final : public engine::core::Controller {};

class MainApp final : public engine::core::App {
protected:
    void app_setup() override {
        auto main_controller = register_controller<MainController>();
        main_controller->after(engine::core::Controller::get<engine::core::EngineControllersEnd>());
    }
};

int main(int argc, char **argv) { return std::make_unique<MainApp>()->run(argc, argv); }
