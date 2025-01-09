#ifndef MOVEMENT_H_
#define MOVEMENT_H_
#include "main.h"
#include "robot.h"


void driver_move(float lPwr, float rPwr);


void spin_left(float lPwr);

void spin_right(float rPwr);

void set_brake_coast();


#endif