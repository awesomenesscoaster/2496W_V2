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
  optical.set_led_pwm(100); 
  pros::Task liftControlTask([]{
    while (true) {
      liftControl();
      pros::delay(10);
    }
  });


  // pros::Task colorSortTask([]{
  //   while (true) {
  //     colorSort();
  //     pros::delay(10);
  //   }
  // });
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
  rotation.set_position(0);
  // brake types
  set_brake_coast(); // chassis coast
  lift.set_brake_mode(MOTOR_BRAKE_HOLD);
  intake.set_brake_mode(MOTOR_BRAKE_COAST);

  controller.clear();

  while (true) {
    double chassis_temp =
        (lf.get_temperature() + lm.get_temperature() + lb.get_temperature() +
         rf.get_temperature() + rm.get_temperature() + rb.get_temperature()) / 6;
    int lift_pos = rotation.get_position();
    counter++;

    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
      autonomous();
    }

    driver();
    print_info(counter, chassis_temp, lift_pos);
    pros::delay(2);
    time += 2;
  }
}

