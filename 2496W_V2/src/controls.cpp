#include "autons.h"
#include "main.h"
#include "movement.h"
#include "piston.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "robot.h"
#include "util.h"

#include "auton_obj.h"
#include <cmath>
#include <iostream>

int abs_sgn(double input) { return input / std::abs(input); }

const int numStates = 4; // number of lift states
int states[numStates] = {0, 30, 40, 200}; // lift degrees

int currState = 0; // current state
int target = 0; // target state

void nextState() {
    currState += 1;
    if (currState == numStates) {
        currState = 0;
    }
    target = states[currState];
}

bool liftControlEnabled = true; // lift control enabled by default

void liftControl() {
    double kp = 1.25;
    if (liftControlEnabled) {
        double error = target - (rotation.get_position() / 100.0);
        double velocity = kp * error;
        lift.move(velocity);
    }
}

void driver()
{
    // ----------- Driver Code ---------- //
    double rPwr, lPwr, rAxis, lAxis, rawLAxis, rawRAxis;

    // deadzones (if value is less than 10, which is usually due to stick drift,
    // set it to 0)
    rawLAxis = controller.get_analog(ANALOG_LEFT_Y);
    rawRAxis = controller.get_analog(ANALOG_RIGHT_X);
    lAxis = abs(rawLAxis) <= 10 ? 0 : rawLAxis;
    rAxis = abs(rawRAxis) <= 10 ? 0 : rawRAxis;

    lPwr = lAxis + rAxis;
    rPwr = lAxis - rAxis;

    driver_move(lPwr, rPwr);

    // ----------- Intake Code ---------- //
    if (controller.get_digital(DIGITAL_R1))
    {
        intake.move(127);
    }
    else if (controller.get_digital(DIGITAL_R2))
    {
        intake.move(-127);
    }
    else
    {
        intake.move(0);
    }

    // ----------- Lift States Code ---------- //
    if (controller.get_digital_new_press(DIGITAL_L1) && controller.get_digital_new_press(DIGITAL_L2))
    {
        if (!liftControlEnabled) {liftControlEnabled = true;}
        nextState();
    }
    else if (controller.get_digital(DIGITAL_L1))
    {
        if (liftControlEnabled) {liftControlEnabled = false;}
        lift.move(127);
    }
    else if (controller.get_digital(DIGITAL_L2))
    {
        if (liftControlEnabled) {liftControlEnabled = false;}
        lift.move(-127);
    }
    else
    {
        lift.move(0);
    }
}
