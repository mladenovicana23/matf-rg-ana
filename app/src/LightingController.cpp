#include <LightingController.hpp>

bool LightingController::loop() {
    const auto platform =
            engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KEY_L).state() ==
        engine::platform::Key::State::JustPressed) { m_point_light_enabled = !m_point_light_enabled; }

    if (platform->key(engine::platform::KEY_J).state() ==
        engine::platform::Key::State::Pressed) { m_point_light_x -= 0.05f; }

    if (platform->key(engine::platform::KEY_K).state() ==
        engine::platform::Key::State::Pressed) { m_point_light_x += 0.05f; }

    if (platform->key(engine::platform::KEY_C).state() ==
        engine::platform::Key::State::JustPressed) { m_point_light_warm = !m_point_light_warm; }

    return true;
}

bool LightingController::point_light_enabled() const { return m_point_light_enabled; }

float LightingController::point_light_x() const { return m_point_light_x; }

bool LightingController::point_light_warm() const { return m_point_light_warm; }