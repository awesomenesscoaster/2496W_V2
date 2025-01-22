#ifndef AUTONS_H_
#define AUTONS_H_

#include "main.h"
#include "robot.h"
#include "movement.h"
#include "auton_obj.h"

#include "pros/misc.h"
#include "pros/rtos.h"
#include <iostream>
#include <fstream>
#include <list>
#include <iostream>

using namespace pros;
using namespace std;

extern void red_safe();

extern void blue_safe();
extern Auton auton_selector(std::vector<Auton> autons); 

extern std::vector<Auton> autons;



#endif