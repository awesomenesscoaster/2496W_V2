#include "main.h"
#include "robot.h"

class Timer {
private:
  int t;

public:
  Timer() { startTime(); }
  void startTime() { t = pros::millis(); }
  int getTime() { return (pros::millis() - t); }
};
