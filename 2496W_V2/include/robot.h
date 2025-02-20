#ifndef ROBOT_H_
#define ROBOT_H_

#include "main.h"
#include "piston.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.hpp"
#include "pros/motors.hpp"

extern pros::Controller controller;

extern pros::Motor lf;
extern pros::Motor lm;
extern pros::Motor lb;
extern pros::Motor rf;
extern pros::Motor rm;
extern pros::Motor rb;

namespace Chassis{
    extern pros::Motor_Group left;
    extern pros::Motor_Group right;
}
extern pros::Motor intake;

extern pros::Motor lift;

extern pros::IMU imu;

extern pros::Rotation rotation;

extern pros::Rotation driveRotation;

extern pros::Optical optical; 

extern pros::ADIDigitalOut clampP;
extern pros::ADIDigitalOut intakeP;
extern pros::ADIDigitalOut spikeP;
extern pros::ADIDigitalOut ptoP;
extern pros::ADIDigitalOut blankP;




namespace glb{
    // This is all jank stuff for display code, pls ignore it lol

    extern std::vector<pros::Motor> motorV;

    extern std::string motor_labels[];

    extern std::string efficiency_a[];

    extern int temps_a[];
}

#endif