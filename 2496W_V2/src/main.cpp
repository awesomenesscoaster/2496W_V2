#include "main.h"
#include "auton_obj.h"
#include "autons.h"
#include "controls.h"
#include "movement.h"
#include "pros/misc.h"
#include "pros/motors.h"
#include "displayC.h"

#include "piston.h"
#include "robot.h"
#include <cmath>
#include <iostream>

using namespace std;

Auton *auton;
string names;

void on_center_button() {}

void initialize() {
  //disp::setupUI();
	//lcd::initialize();
  controller.clear();

  imu.tare();

  driveRotation.set_reversed(true);

  static Auton temp = auton_selector(autons);
  names = temp.get_name();	
  auton = &temp;
  bool color;
  
  if (temp.get_color() == "red"){
    color = true;
  }
  else{
    color = false;
  }
  //const bool color = temp.get_color() == "red"; // True if red, False if Blue
  
  optical.set_led_pwm(100);

  // pros::Task TaskColor(colorSort);

  pros::Task liftControlTask([]{
    while (true) {
      liftControl();
      pros::delay(10);
    }
  });

  pros::Task colorSortTask([color]{
    while (true) {
      colorSort(color);
      pros::delay(10);
    }
  });
  


  // pros::Task updateDisplay([]{
  //   while (true) {
  //     disp::update_battery();
  //     disp::update_optical();
  //     disp::update_diagnostic_log();
  //     disp::update_optical();
  //     pros::delay(100);
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

  

  
  

  // const bool color = names.find("red") != string::npos; // True if red, False if Blue

  while (true) {
    double chassis_temp =
        (lf.get_temperature() + lm.get_temperature() + lb.get_temperature() +
         rf.get_temperature() + rm.get_temperature() + rb.get_temperature()) / 6;
    
    int lift_pos = rotation.get_position();
    

    if (controller.get_digital_new_press(E_CONTROLLER_DIGITAL_UP)) {
      autonomous();
    }
    

    driver();
    print_info(counter, chassis_temp, lift_pos);
    pros::delay(2);

    // if (time%1000==0) disp::temp::updateMotorTemps();
		// if (time%100==0) disp::update_motor_stats();
    // if (time%4000==0) disp::update_match_timer(time/1000);
    
    // if (time%1000==0) disp::update_battery();
    // if (time%4000==0) disp::update_match_timer(time/1000);
    // if (time%500==0) disp::update_diagnostic_log();
    // if (time%1000==0) disp::update_optical();

    time += 2;  
    counter++;
  }
}

