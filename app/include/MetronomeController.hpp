//
// Created by matfrg on 9/18/26.
//

#ifndef METRONOMECONTROLLER_HPP
#define METRONOMECONTROLLER_HPP

#include <engine/core/Engine.hpp>

class MetronomeController final : public engine::core::Controller {
protected:
    bool m_metronome_event_active = false;
    float m_metronome_event_time = 0.0f;

    bool loop() override;

    void update() override;

public:
    bool event_active() const;

    float event_time() const;
};

#endif//METRONOMECONTROLLER_HPP
