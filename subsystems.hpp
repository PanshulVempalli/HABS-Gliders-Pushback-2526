#pragma once

#include "EZ-Template/api.hpp"
#include "api.h"

extern Drive chassis;

extern pros::Motor intake;
extern pros::Motor outtake;

extern pros::ADIDigitalIn back_bumper;
extern pros::Vision vision_sensor;

extern pros::ADIDigitalOut match_loader_piston;
extern pros::ADIDigitalOut wing_piston;
extern pros::ADIDigitalOut mg_piston;