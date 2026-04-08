#include "vex.h"
#include "robot-config.h"

using namespace vex;

competition Competition;

// Defined in autons.cpp
void pre_auton();
void autonomous();
void usercontrol();

int main() {
  vexcodeInit();
  Competition.autonomous(autonomous);
  Competition.drivercontrol(usercontrol);
  pre_auton();
  while (true) {
    wait(100, msec);
  }
}
