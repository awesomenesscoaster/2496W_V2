#ifndef PID_H_
#define PID_H_


#include "main.h"
#include "robot.h"


namespace pid{
    void drive(double target_dist, int timeout=2000, double mult=1.0, double max_speed=127, pros::ADIDigitalOut pis = blankP, int piston_time = 0, int piston_open_time = 0, int exit_time=100);

    double turn_f(double error);

    void turn(double target_deg, int timeout=1000, bool early_exit = true, double multi=1.0, double max_speed=127, int exit_time=75, double tolerance = 0.8);

    void turn_to(double degree_to, int timeout=1000, double multi=1.0, double max_speed=100, int exit_time=100, double tolerance = 0.15);

    void  spin_lift(double position, int timeout=2000, double max_speed=127, int exit_time=100);
}
#endif