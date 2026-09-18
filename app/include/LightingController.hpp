//
// Created by matfrg on 9/18/26.
//

#ifndef LIGHTINGCONTROLLER_HPP
#define LIGHTINGCONTROLLER_HPP

#include <engine/core/Engine.hpp>

class LightingController final : public engine::core::Controller {
protected:
    bool m_point_light_enabled = true;
    float m_point_light_x = 0.0f;
    bool m_point_light_warm = true;

    bool loop() override;

public:
    bool point_light_enabled() const;

    float point_light_x() const;

    bool point_light_warm() const;
};

#endif//LIGHTINGCONTROLLER_HPP
