//
// Created by matfrg on 9/18/26.
//
#include <LightingController.hpp>
#include <MainApp.hpp>
#include <MainController.hpp>
#include <MetronomeController.hpp>

void MainApp::app_setup() {
    auto lighting_controller = register_controller<LightingController>();
    lighting_controller->after(
            engine::core::Controller::get<engine::core::EngineControllersEnd>());

    auto metronome_controller = register_controller<MetronomeController>();
    metronome_controller->after(
            engine::core::Controller::get<engine::core::EngineControllersEnd>());

    auto main_controller = register_controller<MainController>();
    main_controller->after(
            engine::core::Controller::get<engine::core::EngineControllersEnd>());
}