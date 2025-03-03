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
#include <string>

int abs_sgn(double input) { return input / std::abs(input); }

const int numStates = 4; // number of lift states
int states[numStates] = {14, 40, 50, 130}; // lift degrees

int currState = 0; // current state
int target = states[currState]; // target state

void nextState() {
    currState += 1;
    if (currState == numStates) {
        currState = 0;
    }
    target = states[currState];
}

static bool liftControlEnabled = true; // lift control enabled by default

void liftControl() {
    double kp = 2;
    if (liftControlEnabled){
        double error = target - (rotation.get_position() / 100.0);
        double velocity = kp * error;
        lift.move(velocity);
    }
}

static bool interupt = false; 

static bool globalColor; 

void colorSort(bool color){ // true if red, false if blue
    globalColor = color;
    if (color){
        if (optical.get_hue() > 200 && optical.get_hue() < 240){
            interupt = true; 
        }
    }
    else{
        if (optical.get_hue() > 0 && optical.get_hue() < 25){
            interupt = true; 
        }
    } 

    if (interupt){
        delay(200);
        intake.move(-127);
        delay(20);
        interupt = false; 
    }
    pros::delay(25);

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
        if (!interupt){
            intake.move(110);
        }
        else{
            colorSort(globalColor);
        }
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
    if (controller.get_digital(DIGITAL_L1))
    {
        if (liftControlEnabled) {liftControlEnabled = false;}
        lift.move(127);
    }
    else if (controller.get_digital(DIGITAL_L2))
    {
        if (liftControlEnabled) {liftControlEnabled = false;}
        lift.move(-127);
    }
    else if (!liftControlEnabled)
    {
        lift.move(0);
    }
    else
    {
        liftControl();
    }
   

    static bool clampState = false;
    if (controller.get_digital_new_press(DIGITAL_DOWN))
    {
        clampState = !clampState;
        clampP.set_value(clampState);
    }

    static bool intakeState = false;

    if (controller.get_digital_new_press(DIGITAL_A))
    {
        intakeState = !intakeState;
        intakeP.set_value(intakeState);
    }
    
    if (controller.get_digital_new_press(DIGITAL_RIGHT))
    {
        if (!liftControlEnabled) {liftControlEnabled = true;}
        nextState();
    }


    static bool spikeState = false;
    if (controller.get_digital_new_press(DIGITAL_B))
    {
        spikeState = !spikeState;
        spikeP.set_value(spikeState);
    }
}

void print_info(int counter, float chassis_temp, int lift_pos)
{
  if (counter % 50 == 0 && counter % 100 != 0 && counter % 150 != 0)
  {
    controller.print(0, 0, "C: %f   ", float(chassis_temp));
  }
  if (counter % 100 == 0 && counter % 150 != 0)
  {
    controller.print(1, 0, "I: %d, L: %d          ", int(intake.get_temperature()), int(lift.get_temperature()));
  }
  if (counter % 150 == 0 && counter % 300 != 0)
  {
    controller.print(2, 0, "%d    ", driveRotation.get_position());
  }
}

void print_info_auton(int time, double error, double speed)
{
  if (time % 50 == 0 && time % 2000 != 0)
    controller.print(0, 0, "Error: %.2f : %.2f    ", error, speed);
  if (time % 100 == 0 && time % 150 != 0 && time % 2000 != 0)
    controller.print(1, 0, "%.2f : %.2f          ", imu.get_heading());
  if (time % 150 == 0 && time % 100 != 0 && time % 150 != 0 && time % 2000 != 0)
    controller.print(2, 0, "%.2f | %.0f       ", error, time);
}