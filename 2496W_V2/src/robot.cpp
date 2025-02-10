#include "robot.h"
#include "piston.h"
#include "pros/adi.hpp"
#include "pros/imu.hpp"
#include "pros/misc.h"
#include "pros/misc.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"

// -------- All electronic definitions -------- //

pros::Controller controller(pros::E_CONTROLLER_MASTER);

pros::Motor lf(14, MOTOR_GEARSET_06, true, MOTOR_ENCODER_DEGREES);
pros::Motor lm(15, MOTOR_GEARSET_06, true, MOTOR_ENCODER_DEGREES); 
pros::Motor lb(3, MOTOR_GEARSET_06, true, MOTOR_ENCODER_DEGREES);

pros::Motor rf(12, MOTOR_GEARSET_06, false, MOTOR_ENCODER_DEGREES); 
pros::Motor rm(13, MOTOR_GEARSET_06, false, MOTOR_ENCODER_DEGREES); 
pros::Motor rb(1, MOTOR_GEARSET_06, false, MOTOR_ENCODER_DEGREES); 

namespace Chassis{
    pros::Motor_Group left({lf, lm, lb});
    pros::Motor_Group right({rf, rm, rb});
}

pros::Motor intake(11, MOTOR_GEARSET_06, false, MOTOR_ENCODER_DEGREES);
pros::Motor lift(20, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES); //change port on this

pros::IMU imu(2);

pros::Optical optical(19);

pros::Rotation rotation(18);

pros::ADIDigitalOut clampP(1, false);
pros::ADIDigitalOut intakeP(2, true);
pros::ADIDigitalOut spikeP(3, false);
pros::ADIDigitalOut ptoP(4, true);

pros::ADIDigitalOut blankP(6, false);

namespace glb{
    // This is all jank stuff for display code, pls ignore it lol
    std::vector<pros::Motor> motorV = {
        rf,
        rm,
        rb,
        lf,
        lm,
        lb,
        intake,
        lift
    };
   
    std::string motor_labels[] = {
        "R. Top",
        "R. Middle",
        "R. Bottom",
        "L. Top",
        "L. Middle",
        "L. Bottom",
        "Intake",
        "Lift"
    };

    std::string efficiency_a[] = {
        "100%",
        "R. Middle",
        "R. Bottom",
        "Intake 1",
        "Intake 2"
        "L. Top",
        "L. Middle",
        "L. Bottom"
    };

    int temps_a[] = {
        50,
        50,
        50,
        50,
        50,
        50,
        50,
        50
    };
}