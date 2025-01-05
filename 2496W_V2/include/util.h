#ifndef UTIL_H_
#define UTIL_H_

#include "pros/rtos.hpp"

extern bool L1;
extern bool L2;
extern bool R1;
extern bool R2;
extern bool B;
extern bool DOWN;

class Timer
{
private:
    int t{};

public:
    Timer(){startTime();}   
    void startTime() {t = pros::millis();}
    int getTime() {return (pros::millis() - t);}
};

#endif