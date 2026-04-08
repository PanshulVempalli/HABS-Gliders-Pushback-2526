#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen.
brain  Brain;

//The motor constructor takes motors as (port, ratio, reversed), so for example
//motor LeftFront = motor(PORT1, ratio6_1, false);

//Add your devices below, and don't forget to do the same in robot-config.h:


void vexcodeInit( void ) {
  // nothing to initialize
}

// Required by JAR-Template's drive.cpp internal position tracking task.
// Actual robot motion is handled by Chassis my_chassis in autons.cpp.
Drive chassis(
  ZERO_TRACKER_NO_ODOM,
  motor_group(),
  motor_group(),
  PORT7,
  3.25,
  0.6,
  360,
  PORT1, -PORT2,
  PORT3, -PORT4,
  3, 2.75, -2,
  1, -2.75, 5.5
);