#include "main.h"
#include "auton_obj.h"
#include "autons.h"
#include "controls.h"
#include "movement.h"
#include "pros/misc.h"
#include "pros/motors.h"

#include "piston.h"
#include "robot.h"
#include <cmath>
#include <iostream>

using namespace std;

Auton *auton;
string names;

void on_center_button() {}

void initialize() {
  // static Auton temp = auton_selector(autons);
  // names = temp.get_name();	
  // auton = &temp;

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

void competition_initialize() {}

void autonomous() { (*auton).run(); }

// double truncate2(double var){
//   return std::trunc(var * 100)/100;
// }

void opcontrol() {

}

