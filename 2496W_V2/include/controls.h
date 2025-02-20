#ifndef CONTROLS_H_
#define CONTROLS_H_



using namespace std;

void nextState(); 

void colorSort(bool color);

void liftControl(); 

void driver();


void print_info(int counter, float chassis_temp, int lift_pos);

void print_info_auton(int time, double error, double speed);
#endif