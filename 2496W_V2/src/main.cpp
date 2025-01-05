#include "../include/main.h"
#include "../include/auton_obj.h"
#include "../include/autons.h"
#include "../include/controls.h"
#include "../include/movement.h"
#include "../include/pros/misc.h"
#include "../include/pros/motors.h"

#include "../include/piston.h"
#include "../include/robot.h"
#include <cmath>
#include <iostream>

using namespace std;

Auton *auton;
string names;

void on_center_button() {}

void initialize() {
  controller.clear();
  static Auton temp = auton_selector(autons);
  names = temp.get_name();
  auton = &temp;

  rotation.reset_position();
  rotation.set_position(30000);
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
  long long time = 0;
  int counter = 0;
  rotation.reset();
  rotation.set_position(30000);
  // brake types
  set_brake_coast(); // chassis coast
  lift.set_brake_mode(MOTOR_BRAKE_HOLD);
  intake.set_brake_mode(MOTOR_BRAKE_COAST);
  first_stage.set_brake_mode(MOTOR_BRAKE_COAST);

  controller.clear();

  while (true) {
    double chassis_temp =
        (lf.get_temperature() + lm.get_temperature() + lb.get_temperature() +
         rf.get_temperature() + rm.get_temperature() + rb.get_temperature()) /
        6;

    int lift_pos = rotation.get_position();
    print_info(counter, chassis_temp, lift_pos);
    counter++;

    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
      autonomous();
    }

    driver();
    pros::delay(2);
    time += 2;
  }
}
