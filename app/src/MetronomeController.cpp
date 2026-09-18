//
// Created by matfrg on 9/18/26.
//
#include <MetronomeController.hpp>

bool MetronomeController::loop() {
    const auto platform =
            engine::core::Controller::get<engine::platform::PlatformController>();

    if (platform->key(engine::platform::KEY_M).state() ==
        engine::platform::Key::State::JustPressed) {
        m_metronome_event_active = true;
        m_metronome_event_time = 0.0f;
    }

    return true;
}

void MetronomeController::update() {
    if (m_metronome_event_active) {
        auto platform =
                engine::core::Controller::get<engine::platform::PlatformController>();

        m_metronome_event_time += platform->dt();

        if (m_metronome_event_time >= 4.0f) {
            m_metronome_event_active = false;
            m_metronome_event_time = 0.0f;
        }
    }
}

bool MetronomeController::event_active() const { return m_metronome_event_active; }

float MetronomeController::event_time() const { return m_metronome_event_time; }