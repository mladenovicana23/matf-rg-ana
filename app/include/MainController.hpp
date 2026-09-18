//
// Created by matfrg on 9/18/26.
//

#ifndef MAINCONTROLLER_HPP
#define MAINCONTROLLER_HPP

#include <engine/core/Engine.hpp>

class MainController final : public engine::core::Controller {
protected:
    void initialize() override;

    bool loop() override;

    void update_camera();

    void update() override;

    void begin_draw() override;

    void draw() override;

    void end_draw() override;
};

#endif//MAINCONTROLLER_HPP
