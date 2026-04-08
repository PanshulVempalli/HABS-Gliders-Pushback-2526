// ============================================================
//  Special thanks to:
//    - Jess Zarchi (ssejrog) for creating EZ-Template
//      https://github.com/EZ-Robotics/EZ-Template
//    - Purdue ACM SIGBots (BLRS) for creating PROS
//      https://pros.cs.purdue.edu/]
/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////
// ============================================================
//  autons.cpp — Autonomous routines and mechanism helpers
//  Team: HABS Gliders 34071B 
//  Team Member : Panshul Vempalli
//  Team Website : https://habs-gliders-34071b.vercel.app/
//  Contact Info : panshulvempalli@gmail.com
//  Game: VEX Push Back 2025/26
//
//  This file contains:
//    - PID / motion tuning constants (default_constants)
//    - EZ-Template example routines (drive, turn, odom, etc.)
//    - Hardware declarations (motors, sensors, pistons)
//    - Mechanism helper functions (intake, outtake, scoring)
//    - All competition autonomous routines (AWP, Elim, Skills)
//
//  HOW AUTONS WORK:
//    Every chassis.pid_*_set() call starts a motion.
//    chassis.pid_wait() blocks until that motion finishes.
//    chassis.pid_wait_until(x) unblocks early when the robot
//    reaches x, letting the next command overlap the current one.
// ============================================================

#include "main.h"

// These are the max speed caps used throughout all autonomous routines.
// EZ-Template speeds are out of 127 (not percentages).
// DRIVE_SPEED and SWING_SPEED are high because straight moves need power,
// but TURN_SPEED is slightly lower to prevent the robot from overshooting turns.
const int DRIVE_SPEED = 110;
const int TURN_SPEED = 90;
const int SWING_SPEED = 110;

///
// Constants
///
void default_constants() {
  // PID constants control how aggressively the robot corrects errors.
  // P (Proportional) — how hard it reacts to current error. Too high = oscillation.
  // I (Integral)     — corrects for small persistent errors. Usually 0 for drive.
  // D (Derivative)   — dampens the correction to prevent overshooting.
  // Start I          — the error threshold where I starts being applied (only for turns).

  chassis.pid_drive_constants_set(20.0, 0.0, 100.0);         // Forward/reverse drive — high D dampens stopping oscillation
  chassis.pid_heading_constants_set(11.0, 0.0, 20.0);        // Keeps the robot straight while driving (no odom)
  chassis.pid_turn_constants_set(3.0, 0.05, 20.0, 15.0);     // In-place turns — I kicks in at 15 deg to fix final settle
  chassis.pid_swing_constants_set(6.0, 0.0, 65.0);           // Swing (one-side) turns — high D prevents the swing from bouncing
  chassis.pid_odom_angular_constants_set(6.5, 0.0, 52.5);    // Angular correction while using odom for movement
  chassis.pid_odom_boomerang_constants_set(5.8, 0.0, 32.5);  // Angular control specifically for boomerang (curved) paths

  // Exit conditions define when the PID controller decides the motion is "done".
  // Format: (small_timeout, small_error, big_timeout, big_error, velocity_timeout, max_timeout)
  // The robot exits when it's been within small_error for small_timeout,
  // OR within big_error for big_timeout, OR has been moving slowly for velocity_timeout.
  chassis.pid_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_swing_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 500_ms);
  chassis.pid_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 500_ms);
  chassis.pid_odom_turn_exit_condition_set(90_ms, 3_deg, 250_ms, 7_deg, 500_ms, 750_ms);
  chassis.pid_odom_drive_exit_condition_set(90_ms, 1_in, 250_ms, 3_in, 500_ms, 750_ms);

  // Chain constants define how early the robot can exit a motion when chaining
  // multiple moves together. Larger values = more overlap between moves.
  chassis.pid_turn_chain_constant_set(3_deg);
  chassis.pid_swing_chain_constant_set(5_deg);
  chassis.pid_drive_chain_constant_set(3_in);

  // Slew ramps the motor power up from a lower value at the start of a move
  // to prevent wheel slip on hard carpet. The robot starts at 70 power and
  // ramps to full speed after traveling 3 inches/degrees.
  chassis.slew_turn_constants_set(3_deg, 70);
  chassis.slew_drive_constants_set(3_in, 70);
  chassis.slew_swing_constants_set(3_in, 80);

  // odom_turn_bias controls how much turning is prioritized over driving
  // during odom motions. 0.9 means turns are corrected very aggressively.
  // Higher values work better if you have tracking wheels for accurate feedback.
  chassis.odom_turn_bias_set(0.9);

  // look_ahead_set controls how far ahead on the path the robot targets.
  // A larger value = smoother but less precise path following.
  chassis.odom_look_ahead_set(7_in);

  // boomerang_distance_set is how far the "carrot point" (the moving target
  // the robot chases) can be from the final destination.
  chassis.odom_boomerang_distance_set(16_in);

  // boomerang_dlead controls how aggressively the robot curves at the end
  // of a boomerang motion to hit the final heading. Higher = more aggressive.
  chassis.odom_boomerang_dlead_set(0.625);

  // shortest path means turns always go the quickest direction (e.g., -10 deg
  // instead of +350 deg) rather than always going in one direction.
  chassis.pid_angle_behavior_set(ez::shortest);
}

///
// Drive Example
///
void drive_example() {
  // pid_drive_set(distance, speed, slew)
  // distance: positive = forward, negative = backward
  // speed: max power out of 127
  // slew: true = ramp power up at the start (use when distance > slew distance + a few inches)

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();  // Block until this motion finishes before starting the next one

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Turn Example
///
void turn_example() {
  // pid_turn_set(target_angle, speed)
  // Angles are absolute — 90_deg always means "face 90 degrees", not "turn 90 more degrees".
  // To turn relative, use pid_turn_relative_set instead.

  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Combining Turn + Drive
///
void drive_and_turn() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Wait Until and Changing Max Speed
///
void wait_until_change_speed() {
  // pid_wait_until lets you change settings mid-motion without stopping.
  // Here we start slow so the robot doesn't lurch, then speed up after 6 inches
  // when we're confident it's moving straight. pid_speed_max_set updates the
  // speed cap while the PID is still running.

  chassis.pid_drive_set(24_in, 30, true);   // Start at speed 30 to avoid wheel slip
  chassis.pid_wait_until(6_in);             // Once 6 inches in, we're moving cleanly
  chassis.pid_speed_max_set(DRIVE_SPEED);   // Now ramp up to full speed for the rest
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, 30, true);  // Same ramp-up technique going backward
  chassis.pid_wait_until(-6_in);
  chassis.pid_speed_max_set(DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Swing Example
///
void swing_example() {
  // Swing turns pivot on one side of the drive — one set of wheels moves, the other is braked.
  // This creates a wider arc than an in-place turn.
  // The fourth parameter is the speed of the stationary side — a non-zero value
  // lets that side push slightly, creating an even wider arc.

  chassis.pid_swing_set(ez::LEFT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, 45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::LEFT_SWING, 0_deg, SWING_SPEED, 45);
  chassis.pid_wait();
}

///
// Motion Chaining
///
void motion_chaining() {
  // Motion chaining blends consecutive moves together so the robot never fully
  // stops between them — it exits each motion while still moving slightly.
  // This saves time and makes paths look smoother.
  // Use pid_wait_quick_chain() instead of pid_wait() between chained moves.
  // The LAST motion in a chain must always end with a normal pid_wait()
  // so the robot is fully settled before the function returns.

  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();  // Full stop before the chain starts

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();  // Exit early, blend into next move

  chassis.pid_turn_set(-45_deg, TURN_SPEED);
  chassis.pid_wait_quick_chain();  // Exit early, blend into next move

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();  // Full settle before driving back

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Auto that tests everything
///
void combining_movements() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_turn_set(45_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_swing_set(ez::RIGHT_SWING, -45_deg, SWING_SPEED, 45);
  chassis.pid_wait();

  chassis.pid_turn_set(0_deg, TURN_SPEED);
  chassis.pid_wait();

  chassis.pid_drive_set(-24_in, DRIVE_SPEED, true);
  chassis.pid_wait();
}

///
// Interference example
///
// tug() attempts to drive backward. If the robot gets stuck (chassis.interfered == true),
// it resets its encoders, nudges backward at low power for 1 second, then retries.
// This handles being pushed or pinned by another robot — it won't just give up.
void tug(int attempts) {
  for (int i = 0; i < attempts - 1; i++) {
    printf("i - %i", i);
    chassis.pid_drive_set(-12_in, 127);
    chassis.pid_wait();

    if (chassis.interfered) {
      // The robot didn't reach its target — something blocked it.
      // Reset encoders and apply a slow nudge to escape, then retry.
      chassis.drive_sensor_reset();
      chassis.pid_drive_set(-2_in, 20);
      pros::delay(1000);
    } else {
      // Robot drove back successfully — no interference, exit early.
      return;
    }
  }
}

// Drives forward, then checks chassis.interfered to decide what to do next.
// chassis.interfered is set by EZ-Template when the robot doesn't reach its
// target within the expected time — usually means something hit or blocked it.
void interfered_example() {
  chassis.pid_drive_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  if (chassis.interfered) {
    tug(3);  // Try to escape — something blocked the drive forward
    return;
  }

  // No interference — normal path continues with a turn
  chassis.pid_turn_set(90_deg, TURN_SPEED);
  chassis.pid_wait();
}

///
// Odom Drive PID
///
void odom_drive_example() {
  // pid_odom_set works the same as pid_drive_set but uses odometry for error correction.
  // Regular drive PID only reads wheel encoders, so pushing the robot off course mid-move
  // is not corrected. Odom-based drive reads real X/Y position and steers back on track.

  chassis.pid_odom_set(24_in, DRIVE_SPEED, true);
  chassis.pid_wait();

  chassis.pid_odom_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();

  chassis.pid_odom_set(-12_in, DRIVE_SPEED);
  chassis.pid_wait();
}

///
// Odom Pure Pursuit
///
void odom_pure_pursuit_example() {
  // Pure Pursuit follows a list of waypoints in order, smoothing the path between them.
  // The robot doesn't stop at each waypoint — it looks ahead to the next one and steers
  // toward it, creating a curved path instead of sharp point-to-point moves.
  // Great for sweeping field blocks without stopping.

  // Drive to (0, 30) passing through (6, 10) and (0, 20) on the way
  chassis.pid_odom_set({{{6_in, 10_in}, fwd, DRIVE_SPEED},
                        {{0_in, 20_in}, fwd, DRIVE_SPEED},
                        {{0_in, 30_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait();

  // Return to start driving backward
  chassis.pid_odom_set({{0_in, 0_in}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Odom Pure Pursuit Wait Until
///
void odom_pure_pursuit_wait_until_example() {
  // pid_wait_until_index(n) pauses execution until the robot passes the nth waypoint
  // in the list (0-indexed), without stopping the motion. This lets you trigger
  // mechanisms mid-path — e.g. start the intake exactly when passing a certain point.

  chassis.pid_odom_set({{{0_in, 24_in}, fwd, DRIVE_SPEED},
                        {{12_in, 24_in}, fwd, DRIVE_SPEED},
                        {{24_in, 24_in}, fwd, DRIVE_SPEED}},
                       true);
  chassis.pid_wait_until_index(1);  // Fires when robot passes (12, 24) — second waypoint
  // Intake.move(127);  // Example: start intake here mid-path
  chassis.pid_wait();
  // Intake.move(0);  // Example: stop intake after full path completes
}

///
// Odom Boomerang
///
void odom_boomerang_example() {
  // Boomerang adds a target heading to the destination so the robot arrives
  // facing a specific direction — useful when you need to be aligned for scoring.
  // The robot curves to hit both the position and the angle at the same time.

  // Drive to (0, 24) and arrive facing 45 degrees
  chassis.pid_odom_set({{0_in, 24_in, 45_deg}, fwd, DRIVE_SPEED},
                       true);
  chassis.pid_wait();

  // Return to (0, 0) facing 0 degrees, driving in reverse
  chassis.pid_odom_set({{0_in, 0_in, 0_deg}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Odom Boomerang Injected Pure Pursuit
///
void odom_boomerang_injected_pure_pursuit_example() {
  // Combines Boomerang and Pure Pursuit — the first waypoint uses a target heading
  // (Boomerang), but intermediate points are followed smoothly (Pure Pursuit).
  // This lets you arrive at a specific angle AND follow a curved path to get there.

  chassis.pid_odom_set({{{0_in, 24_in, 45_deg}, fwd, DRIVE_SPEED},  // Boomerang waypoint — arrive at 45 deg
                        {{12_in, 24_in}, fwd, DRIVE_SPEED},          // Pure Pursuit — pass through
                        {{24_in, 24_in}, fwd, DRIVE_SPEED}},         // Pure Pursuit — final destination
                       true);
  chassis.pid_wait();

  chassis.pid_odom_set({{0_in, 0_in, 0_deg}, rev, DRIVE_SPEED},
                       true);
  chassis.pid_wait();
}

///
// Calculate the offsets of your tracking wheels
///
void measure_offsets() {
  // This routine spins the robot in place repeatedly and measures how much each
  // tracking wheel moves per radian of rotation. That measurement is the wheel's
  // physical distance from the robot's centre of rotation.
  // EZ-Template uses this offset to correctly calculate X/Y position in odom.
  // Run this once and paste the printed values into your tracker constructors.

  int iterations = 10;  // More iterations = more averaged, more accurate result

  double l_offset = 0.0, r_offset = 0.0, b_offset = 0.0, f_offset = 0.0;

  // Reset all trackers before starting so the measurements begin from zero
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->reset();
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->reset();
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->reset();
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->reset();

  for (int i = 0; i < iterations; i++) {
    // Reset chassis state each iteration for a clean baseline
    chassis.pid_targets_reset();
    chassis.drive_imu_reset();
    chassis.drive_sensor_reset();
    chassis.drive_brake_set(MOTOR_BRAKE_HOLD);
    chassis.odom_xyt_set(0_in, 0_in, 0_deg);
    double imu_start = chassis.odom_theta_get();

    // Alternate between 90 and 270 degrees each run to average out any asymmetry
    // in the robot's turning (e.g. one side has more friction than the other)
    double target = i % 2 == 0 ? 90 : 270;

    // Turn at half power using raw mode — bypasses PID angle correction so
    // the measurement reflects actual mechanical behavior, not software fixes
    chassis.pid_turn_set(target, 63, ez::raw);
    chassis.pid_wait();
    pros::delay(250);  // Let the robot fully settle before reading sensors

    // Calculate how many radians the robot actually turned
    double t_delta = util::to_rad(fabs(util::wrap_angle(chassis.odom_theta_get() - imu_start)));

    // Read how far each tracking wheel moved during that turn.
    // A nullptr check prevents crashes when a tracker isn't installed.
    double l_delta = chassis.odom_tracker_left != nullptr ? chassis.odom_tracker_left->get() : 0.0;
    double r_delta = chassis.odom_tracker_right != nullptr ? chassis.odom_tracker_right->get() : 0.0;
    double b_delta = chassis.odom_tracker_back != nullptr ? chassis.odom_tracker_back->get() : 0.0;
    double f_delta = chassis.odom_tracker_front != nullptr ? chassis.odom_tracker_front->get() : 0.0;

    // wheel travel / angle turned = radius = distance from centre of rotation
    l_offset += l_delta / t_delta;
    r_offset += r_delta / t_delta;
    b_offset += b_delta / t_delta;
    f_offset += f_delta / t_delta;
  }

  // Average across all iterations to reduce noise from individual runs
  l_offset /= iterations;
  r_offset /= iterations;
  b_offset /= iterations;
  f_offset /= iterations;

  // Push the calculated offsets back into each tracker so odom is accurate
  if (chassis.odom_tracker_left != nullptr) chassis.odom_tracker_left->distance_to_center_set(l_offset);
  if (chassis.odom_tracker_right != nullptr) chassis.odom_tracker_right->distance_to_center_set(r_offset);
  if (chassis.odom_tracker_back != nullptr) chassis.odom_tracker_back->distance_to_center_set(b_offset);
  if (chassis.odom_tracker_front != nullptr) chassis.odom_tracker_front->distance_to_center_set(f_offset);
}

// . . .
// Make your own autonomous functions here! ---------------------------------------------------------------------------------------------------------
// . . .



// ---------------- MECHANISMS ----------------
// Port 10 = intake motor, port 8 = outtake motor.
// Both use green 200 RPM cartridges. false = not reversed (forward voltage = intake direction).
pros::Motor intake(10, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);
pros::Motor outtake(8, pros::v5::MotorGears::green, pros::v5::MotorUnits::degrees);

// ---------------- SENSORS ----------------
// back_bumper is a digital limit switch on ADI port H — reads true when pressed.
// Used during scoring to confirm the robot is physically against the goal.
// vision_sensor is on smart port 9 — used to detect and align to colored game objects.
pros::ADIDigitalIn back_bumper('H');
pros::Vision vision_sensor(9);

// ---------------- PNEUMATICS ----------------
// All three are digital output pistons — set_value(true) extends, false retracts.
// ADI ports A, B, C are the three-wire ports on the brain/expander.
pros::ADIDigitalOut match_loader_piston('A');
pros::ADIDigitalOut wing_piston('B');
pros::ADIDigitalOut mg_piston('C');

// ---------------- HELPERS ----------------

// intake_on: runs intake at 100% forward AND outtake at 10% (1200/12000 mV).
// The slow outtake spin acts as a soft barrier — it keeps scored blocks from
// rolling back down out of the uptake zone while new ones are being pulled in.
void intake_on() { intake.move_voltage(12000); outtake.move_voltage(1200); }

// intake_off: brakes both motors so everything stops cleanly.
// Called before scoring so blocks settle into the outtake mechanism properly.
void intake_off() { intake.brake(); outtake.brake(); }

// score_middle: fires the outtake at full power for 500ms then brakes.
// 500ms is tuned to be long enough to push blocks fully into the middle goal.
void score_middle() {
    outtake.move_voltage(12000);
    pros::delay(500);
    outtake.brake();
}

// score_low: runs intake in reverse to push blocks out the back into the low goal.
// No delay needed — the caller controls timing by how long they leave it running.
void score_low() { intake.move_voltage(-12000); }

// score_long: fires outtake in reverse at full power for 1000ms then brakes.
// Reverse direction pushes blocks backward into long goals (the robot backs into them).
// 1000ms is long enough to fully clear the outtake channel.
void score_long() {
    outtake.move_voltage(-12000);
    pros::delay(1000);
    outtake.brake();
}

// Pneumatic helpers — simple wrappers so auton code reads like English.
void mg_on() { mg_piston.set_value(true); }    // Extend the MG piston
void mg_off() { mg_piston.set_value(false); }  // Retract the MG piston
void wings_on() { wing_piston.set_value(true); }
void wings_off() { wing_piston.set_value(false); }
void matchload_on() { match_loader_piston.set_value(true); }   // Open matchloader gate
void matchload_off() { match_loader_piston.set_value(false); } // Close matchloader gate

// ---------------- INTAKE ANTI-JAM ----------------
// Fires a brief reverse burst then resumes forward to dislodge a block that's
// stuck in the intake channel mid-sweep. Without this, one stuck block stalls
// the intake motor and the robot stops collecting during long sweeps.
// The outtake mirrors the intake state: runs slow during forward, brakes during reverse.
void intake_anti_jam() {
    intake.move_voltage(12000); outtake.move_voltage(1200);  // Resume forward + hold barrier
    pros::delay(150);
    intake.move_voltage(-9600); outtake.brake();             // Reverse burst to kick stuck block loose
    pros::delay(80);
    intake.move_voltage(12000); outtake.move_voltage(1200);  // Back to forward + barrier
}

// ---------------- VISION ALIGN ----------------
// Continuously steers the robot left/right until the detected object is centred
// in the vision sensor's field of view (within 8 pixels of centre = 158px on a 320px frame).
// strength controls how hard it steers per pixel of error — lower = gentler correction.
// If no object is detected, it sweeps slowly to find one.
// Falls back after max_time milliseconds to prevent getting stuck.
void vision_align_smart(double strength = 0.25, int max_time = 500) {
    int timer = 0;
    while (timer < max_time) {
        auto obj = vision_sensor.get_by_sig(0, 1);  // Get the first object matching signature 1
        if (obj.signature != 0) {
            int error = obj.x_middle_coord - 158;   // 158 is horizontal centre of the 320px frame
            if (std::abs(error) < 8) break;         // Close enough — stop correcting
            double turn = error * strength;
            chassis.drive_set(turn, -turn);          // Steer left or right proportionally
        } else {
            chassis.drive_set(30, -30);              // No object found — slow sweep to search
        }
        pros::delay(20);
        timer += 20;
    }
    chassis.drive_set(0, 0);
}

// ---------------- SCORING HELPERS ----------------

// score_long_double: scores the long goal twice with a press between taps.
// The first score fires blocks in; the robot then nudges against the goal so
// any blocks that didn't fully enter get pushed in on the second shot.
// The 5cm forward pull-out at the end lets the robot leave cleanly without
// dragging blocks back out with it.
void score_long_double() {
    score_long();
    pros::delay(200);
    chassis.drive_set(-30, -30);   // Press lightly against the goal between shots
    pros::delay(100);
    chassis.drive_set(0, 0);
    score_long();
    chassis.pid_drive_set(5_cm, 60);  // Pull out cleanly so blocks stay scored
    chassis.pid_wait();
}

// ---------------- BUMPER SAFE ----------------

// score_long_safe: the full reliability sequence for match autons.
// Step 1 — vision align: steers until the goal is centred in the camera.
// Step 2 — bumper approach: reverses slowly until the back bumper confirms
//           contact with the goal. Using both sensors means even if vision is
//           slightly off, the bumper catches the mistake.
// Step 3 — retry: if the bumper never triggered (missed the goal), nudge the
//           angle by 10 degrees and try the approach again once.
// Step 4 — double-tap score for deeper, more reliable scoring.
void score_long_safe() {
    pros::delay(100);                // Let the robot settle before vision reads
    vision_align_smart();
    pros::delay(100);                // Let blocks settle after aligning before approach
    int timeout = 1200, timer = 0;
    while (!back_bumper.get_value() && timer < timeout) {
        chassis.drive_set(-40, -40); // Slow reverse approach toward goal
        pros::delay(20);
        timer += 20;
    }
    chassis.drive_set(0, 0);
    if (!back_bumper.get_value()) {  // Bumper never triggered — we missed the goal
        chassis.pid_turn_relative_set(-10, 110);  // Small angle correction
        chassis.pid_wait();
        timer = 0;
        while (!back_bumper.get_value() && timer < timeout) {
            chassis.drive_set(-40, -40);
            pros::delay(20);
            timer += 20;
        }
        chassis.drive_set(0, 0);
    }
    pros::delay(100);
    score_long_double();             // Double-tap for deeper scoring
}

// score_long_bumper_only: skills-only version — no vision sensor because the
// robot faces away from the goal during reverse scoring and the camera can't see it.
// Reverses slowly until the bumper confirms contact, then double-tap scores.
// Retry logic nudges the angle 5 degrees if the first approach misses.
void score_long_bumper_only() {
    pros::delay(100);                // Let blocks settle before approach
    int timeout = 1200, timer = 0;
    while (!back_bumper.get_value() && timer < timeout) {
        chassis.drive_set(-40, -40);
        pros::delay(20);
        timer += 20;
    }
    chassis.drive_set(0, 0);
    if (!back_bumper.get_value()) {  // Missed — small angle correction and retry
        chassis.pid_turn_relative_set(-5_deg, TURN_SPEED);
        chassis.pid_wait();
        timer = 0;
        while (!back_bumper.get_value() && timer < timeout) {
            chassis.drive_set(-40, -40);
            pros::delay(20);
            timer += 20;
        }
        chassis.drive_set(0, 0);
    }
    pros::delay(100);
    score_long_double();
}

// score_middle_bumper_only: same bumper-approach logic but for the middle goal.
// Timeout is 1000ms instead of 1200ms since the middle goal is closer.
// After confirming contact, fires score_middle() then pulls out 5cm so the
// robot doesn't drag blocks back with it when it drives away.
void score_middle_bumper_only() {
    pros::delay(100);
    int timeout = 1000, timer = 0;
    while (!back_bumper.get_value() && timer < timeout) {
        chassis.drive_set(-40, -40);
        pros::delay(20);
        timer += 20;
    }
    chassis.drive_set(0, 0);
    pros::delay(100);
    score_middle();
    chassis.pid_drive_set(5_cm, 60);  // Pull out cleanly after scoring
    chassis.pid_wait();
}

// try_score_long_with_timeout: skills-only helper that returns false if the
// robot misses the goal entirely. The caller can then skip the score and
// continue the route rather than stalling for 2+ seconds on a missed approach.
bool try_score_long_with_timeout(int timeout = 1200) {
    int timer = 0;
    while (!back_bumper.get_value() && timer < timeout) {
        chassis.drive_set(-50, -50);
        pros::delay(20);
        timer += 20;
    }
    chassis.drive_set(0, 0);
    if (back_bumper.get_value()) {
        score_long_double();
        return true;   // Goal confirmed and scored
    }
    return false;      // Missed — caller decides what to do next
}

// ---------------- SKILLS AUTONOMOUS ----------------

void auton_prog_skills() {
    wings_off();
    matchload_off();

    // ── PHASE 1: Drive forward and collect floor blocks ───────────────────
    // intake_on runs the intake + slow outtake barrier simultaneously.
    // mg_on extends the MG piston to guide blocks into the intake channel.
    intake_on();
    mg_on();
    chassis.pid_drive_set(70_cm, DRIVE_SPEED, true);  // TUNE: distance to first block group
    chassis.pid_wait();

    // ── PHASE 2: Turn right so back faces middle goal ─────────────────────
    chassis.pid_turn_set(90_deg, TURN_SPEED);
    chassis.pid_wait();

    // ── PHASE 3: Reverse to position between long goal and matchloader ─────
    chassis.pid_drive_set(-40_cm, DRIVE_SPEED);
    chassis.pid_wait();

    // ── PHASE 4: Extend matchloader, small left turn to straighten ─────────
    matchload_on();
    chassis.pid_turn_set(85_deg, TURN_SPEED);  // TUNE: small tweak to align with matchloader
    chassis.pid_wait();

    // ── PHASE 5: Drive FORWARD into matchloader (intake first) ────────────
    // Driving forward means the intake side enters the matchloader, pulling
    // triballs in automatically as the piston feeds them. Slower speed (60)
    // gives the intake time to grip each ball without bouncing.
    chassis.pid_drive_set(50_cm, 60);          // TUNE: distance to reach matchloader
    chassis.pid_wait();
    // Wiggle back and forth to shake any stuck balls loose before closing the piston
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // ── PHASE 6: Back away, turn, wall align against left wall ────────────
    // Turning to 180° faces the robot away from the top wall.
    // Driving into the left wall at -70 power squares up the heading so the
    // long cross-field drive in phase 7 stays straight.
    chassis.pid_drive_set(-30_cm, DRIVE_SPEED);
    chassis.pid_wait();
    chassis.pid_turn_set(180_deg, TURN_SPEED);
    chassis.pid_wait();

    // Wall align — push outtake against left wall to square up
    chassis.drive_set(-70, -70);
    pros::delay(300);
    chassis.drive_set(0, 0);
    pros::delay(50);
    chassis.pid_drive_set(5_cm, 40);           // Pull off wall so wheels can grip cleanly
    chassis.pid_wait();

    // ── PHASE 7: Re-align and reverse across field (split drive reduces drift) ──
    // Split into two legs with a micro-straighten between them.
    // The micro-straighten (pid_turn_relative_set 0_deg) re-confirms the current
    // heading mid-field so accumulated drift doesn't compound over 120+ cm.
    chassis.pid_turn_set(90_deg, TURN_SPEED);
    chassis.pid_wait();
    chassis.pid_drive_set(-80_cm, DRIVE_SPEED, true);  // First leg of cross-field drive
    chassis.pid_wait();
    chassis.pid_turn_relative_set(0_deg, 60);          // Micro-straighten: re-settles heading
    chassis.pid_wait();
    chassis.pid_drive_set(-40_cm, DRIVE_SPEED, true);  // Second leg
    chassis.pid_wait();

    // ── PHASE 8: Turn toward bottom long goal area ────────────────────────
    chassis.pid_turn_set(180_deg, TURN_SPEED);
    chassis.pid_wait();

    // ── PHASE 9: Drive to position, wall align against bottom wall ────────
    chassis.pid_drive_set(50_cm, DRIVE_SPEED, true);   // TUNE: positioning distance
    chassis.pid_wait();

    // Wall align — push outtake against bottom wall to square up before scoring
    chassis.drive_set(-60, -60);
    pros::delay(300);
    chassis.drive_set(0, 0);
    pros::delay(50);
    chassis.pid_drive_set(5_cm, 40);                   // Pull off wall
    chassis.pid_wait();

    // ── PHASE 10: Score left long goal (bumper verified) ──────────────────
    // intake_off first so any blocks in the intake channel fall into the outtake
    // and settle before the scoring sequence fires.
    intake_off();
    chassis.pid_drive_set(-20_cm, 60);                 // TUNE: get close to goal opening
    chassis.pid_wait();
    score_long_bumper_only();                          // Slow approach + bumper contact + double-tap score

    // ── PHASE 11: Drive FORWARD to matchloader, load balls ────────────────
    chassis.pid_drive_set(30_cm, 60);                  // TUNE: distance to matchloader
    chassis.pid_wait();
    matchload_on();
    pros::delay(900);                                  // TUNE: matchload duration
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // ── PHASE 12: Reverse and score left long goal again (bumper verified) ─
    // Second fill of the same goal — scores newly loaded blocks on top.
    intake_off();
    chassis.pid_drive_set(-20_cm, 60);                 // TUNE: get close to goal opening
    chassis.pid_wait();
    score_long_bumper_only();
    // Left long goal is now completely full.

    // ── PHASE 13: Turn, drive into top wall to straighten ─────────────────
    // Facing 270° (top wall) then driving into it squares up the heading
    // before the long park-zone sweep in phase 14.
    intake_on();
    chassis.pid_turn_set(270_deg, TURN_SPEED);         // Face top wall
    chassis.pid_wait();

    // Wall align — push intake side against top wall to square up
    chassis.drive_set(70, 70);
    pros::delay(400);
    chassis.drive_set(0, 0);
    pros::delay(50);
    chassis.pid_drive_set(-5_cm, 40);                  // Pull off wall
    chassis.pid_wait();

    // ── PHASE 14: Drive intake-first through top blue park zone ───────────
    // Sweeps through the zone collecting all loose blocks.
    // An anti-jam pulse fires halfway through to prevent any block from stalling
    // the intake channel and interrupting the sweep.
    chassis.pid_drive_set(130_cm, DRIVE_SPEED, true);  // TUNE: sweep distance
    chassis.pid_wait_until(65_cm);                     // Halfway — trigger anti-jam
    intake_anti_jam();
    chassis.pid_wait();

    // ── PHASE 15: Back up to right edge of park zones, turn to face field ─
    chassis.pid_drive_set(-55_cm, DRIVE_SPEED);        // TUNE: reverse to right edge
    chassis.pid_wait();
    chassis.pid_turn_set(0_deg, TURN_SPEED);
    chassis.pid_wait();

    // ── PHASE 16: Drive intake-first to second set of 4 floor blocks ──────
    intake_on();
    chassis.pid_drive_set(60_cm, DRIVE_SPEED, true);   // TUNE: distance to upper-right floor blocks
    chassis.pid_wait();

    // ── PHASE 17: Turn to align outtake with middle goal ──────────────────
    chassis.pid_turn_set(225_deg, TURN_SPEED);         // TUNE: angle so outtake faces middle goal
    chassis.pid_wait();

    // ── PHASE 18: Score middle goal (bumper verified) ─────────────────────
    // mg_off retracts the MG piston so it doesn't physically block the goal entry.
    mg_off();
    intake_off();
    chassis.pid_drive_set(-25_cm, 60);                 // TUNE: get close to middle goal opening
    chassis.pid_wait();
    score_middle_bumper_only();                        // Bumper approach + score + pull out
    mg_on();

    // ── PHASE 19: Reverse out to align with top-right matchloader ─────────
    chassis.pid_drive_set(-50_cm, DRIVE_SPEED);        // TUNE: reverse to clear goal + align with matchloader
    chassis.pid_wait();

    // ── PHASE 20: Turn to face top-right matchloader (intake first) ───────
    chassis.pid_turn_set(135_deg, TURN_SPEED);         // TUNE: angle to top-right matchloader
    chassis.pid_wait();

    // ── PHASE 21: Drive intake-first into top-right matchloader ───────────
    // Same as phase 5 — intake-first entry, slow speed, wiggle to clear stuck balls.
    intake_on();
    matchload_on();
    chassis.pid_drive_set(100_cm, 60);                 // TUNE: distance into top-right matchloader
    chassis.pid_wait();
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // ── PHASE 22: Reverse toward bottom-right (split drive reduces drift) ──
    // Split into two legs with a heading correction between them,
    // same technique as phase 7. The correction prevents accumulated drift
    // from pushing the robot into a wall before it reaches the right goal.
    chassis.pid_drive_set(-80_cm, DRIVE_SPEED, true);  // First leg out of matchloader
    chassis.pid_wait();
    chassis.pid_turn_set(135_deg, TURN_SPEED);         // TUNE: heading check mid-field
    chassis.pid_wait();
    chassis.pid_drive_set(-70_cm, DRIVE_SPEED, true);  // Second leg toward right wall
    chassis.pid_wait();

    // ── PHASE 23: Wall align against right wall ───────────────────────────
    // Facing 90° means the outtake (back of robot) points toward the right wall.
    chassis.pid_turn_set(90_deg, TURN_SPEED);
    chassis.pid_wait();

    // Wall align — push outtake against right wall to square up
    chassis.drive_set(-70, -70);
    pros::delay(300);
    chassis.drive_set(0, 0);
    pros::delay(50);
    chassis.pid_drive_set(5_cm, 40);                   // Pull off wall
    chassis.pid_wait();

    // ── PHASE 24: Score right long goal (bumper verified) ─────────────────
    intake_off();
    chassis.pid_drive_set(-20_cm, 60);                 // TUNE: get close to goal opening
    chassis.pid_wait();
    score_long_bumper_only();

    // ── PHASE 25: Drive forward to bottom-right matchloader, load balls ───
    chassis.pid_drive_set(30_cm, 60);                  // TUNE: forward to bottom-right matchloader
    chassis.pid_wait();
    matchload_on();
    pros::delay(900);                                  // TUNE: matchload duration
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // ── PHASE 26: Reverse and score right long goal again (bumper verified) ─
    intake_off();
    chassis.pid_drive_set(-20_cm, 60);                 // TUNE: get close to goal opening
    chassis.pid_wait();
    score_long_bumper_only();
    // Right long goal is now completely full.

    // ── PHASE 27: Turn toward bottom wall to begin park sequence ──────────
    chassis.pid_turn_set(180_deg, TURN_SPEED);         // Face south (bottom wall)
    chassis.pid_wait();

    // ── PHASE 28: Drive into bottom wall to square up ─────────────────────
    // Driving forward into the wall aligns the robot before the final park turn.
    chassis.drive_set(70, 70);
    pros::delay(400);
    chassis.drive_set(0, 0);
    pros::delay(50);

    // ── PHASE 29: Back up slightly, turn to align with park zone ──────────
    chassis.pid_drive_set(-10_cm, DRIVE_SPEED);        // Pull off wall
    chassis.pid_wait();
    chassis.pid_turn_set(270_deg, TURN_SPEED);         // Face park zone (west)
    chassis.pid_wait();

    // ── PHASE 30: Drive over zone — clear blocks and park ─────────────────
    // intake_on + outtake reverse both run simultaneously to eject any blocks
    // sitting in the zone that might stop the robot from parking cleanly.
    // The robot drives far enough to be fully inside the zone at the end.
    intake_on();
    outtake.move_voltage(-12000);                      // Eject any zone blocks forward
    chassis.pid_drive_set(60_cm, DRIVE_SPEED, true);   // TUNE: distance to park fully inside zone
    chassis.pid_wait();
    intake_off();
    outtake.brake();
    // Robot is now parked in the bottom park zone. Autonomous complete.
}


// ---------------- AUTONOMOUS ROUTINES ----------------

void auton_elim_left() {
    wings_off(); matchload_off();
    intake_on(); mg_on();

    // 1. DRIVE TO FIRST LONG GOAL
    // pid_wait_until lets the robot start turning before fully stopping — saves time.
    // The chain of drive → turn → drive → turn → drive positions the robot
    // so its back (outtake) faces the long goal for scoring.
    chassis.pid_drive_set(60_cm, 110); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(-90, 90); chassis.pid_wait_until(-85);
    chassis.pid_drive_set(75_cm, 110); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(-70, 90); chassis.pid_wait();
    chassis.pid_drive_set(35_cm, 80); chassis.pid_wait();

    // 2. SCORE FIRST LONG GOAL
    // intake_off so blocks settle into outtake before scoring.
    // score_long_safe: vision align + bumper approach + double-tap score.
    intake_off();
    score_long_safe();

    // 3. MATCHLOAD
    // Back up to create room, then drive forward into the matchloader with intake on.
    // 300ms delay gives the piston time to extend before the intake starts pulling.
    // Wiggle sequence shakes any stuck balls off the piston teeth before retracting.
    chassis.pid_drive_set(-25_cm, 110); chassis.pid_wait_until(-5_cm);
    intake_on();
    chassis.pid_drive_set(45_cm, 110); chassis.pid_wait();
    matchload_on(); pros::delay(300);
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // 4. DRIVE TO SECOND LONG GOAL
    // Mirror of step 1 — position the robot with its back facing the second goal.
    chassis.pid_drive_set(-45_cm, 110); chassis.pid_wait_until(-5_cm);
    chassis.pid_turn_set(-90, 90); chassis.pid_wait();
    chassis.pid_drive_set(35_cm, 80); chassis.pid_wait();

    // 5. SCORE SECOND LONG GOAL
    intake_off();
    score_long_safe();

    // 6. WING PUSH
    // Wings extend to sweep extra blocks toward the goal.
    // The S-turn (turn to 45, drive a bit, turn to -45) creates a sweeping arc
    // that knocks field blocks toward the goal in one fluid motion.
    // Final drive_set instead of pid_drive_set for a harder, more consistent shove.
    chassis.pid_drive_set(-20_cm, 110); chassis.pid_wait_until(-5_cm);
    wings_on();
    chassis.pid_turn_set(45, 90); chassis.pid_wait_until(40);
    chassis.pid_drive_set(10_cm, 80); chassis.pid_wait_until(2_cm);
    chassis.pid_turn_set(-45, 90); chassis.pid_wait_until(-40);
    chassis.drive_set(80, 80);  // Constant voltage push — more consistent than PID on final shove
    pros::delay(250);
    chassis.drive_set(0, 0);
    intake_off();
}

void auton_awp_right() {
    wings_off(); matchload_off();
    intake_on(); mg_on();

    // 1. FIRST MATCHLOAD
    // Drive toward the matchloader position, turn to face it, then drive in slowly (60)
    // so the intake pulls triballs in without bouncing them off the walls.
    // Reduced to 85cm (was 100cm) to reduce overshoot risk at the matchloader.
    chassis.pid_drive_set(45_cm, 110); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(-90, 90); chassis.pid_wait();
    matchload_on(); chassis.pid_drive_set(85_cm, 60); chassis.pid_wait();
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // 2. SCORE FIRST LONG GOAL
    // Reverse out of the matchloader area, turn off intake so blocks settle,
    // then use score_long_safe (vision align + bumper + double-tap).
    chassis.pid_drive_set(-95_cm, 110); chassis.pid_wait_until(-3_cm);
    intake_off();
    score_long_safe();

    // 3. NAVIGATE TO MIDDLE GOAL
    // A chain of small drives and turns threads the robot through the field
    // to reach the middle goal position without hitting field elements.
    intake_on();
    chassis.pid_drive_set(10_cm, 80); chassis.pid_wait_until(2_cm);
    chassis.pid_turn_set(-115, 90); chassis.pid_wait_until(-110);
    chassis.pid_drive_set(30_cm, 110); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(25, 90); chassis.pid_wait_until(20);
    chassis.pid_drive_set(35_cm, 80); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(35, 90); chassis.pid_wait();

    // 4. SCORE MIDDLE GOAL
    // mg_off retracts the MG piston so it doesn't physically block the goal entry.
    // Drive in slowly, score, then pull out 5cm so the robot leaves cleanly.
    mg_off();
    intake_off();
    chassis.pid_drive_set(20_cm, 60); chassis.pid_wait();
    score_middle();
    chassis.pid_drive_set(5_cm, 60); chassis.pid_wait();  // Pull out cleanly
    mg_on();

    // 5. SECOND MATCHLOAD
    // Same approach as step 1 — drive toward, turn, drive in slowly.
    // Reduced turn speed to 90 (was 127) to reduce overshoot on the approach turn.
    intake_on();
    chassis.pid_drive_set(90_cm, 110); chassis.pid_wait_until(10_cm);
    chassis.pid_turn_set(40, 90); chassis.pid_wait();
    matchload_on(); chassis.pid_drive_set(85_cm, 60); chassis.pid_wait();
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // 6. SCORE SECOND LONG GOAL
    // Reverse back to the goal and score with full reliability sequence.
    chassis.pid_drive_set(-95_cm, 110); chassis.pid_wait_until(-2_cm);
    intake_off();
    score_long_safe();
}

void auton_elim_right() {
    wings_off(); matchload_off();
    intake_on(); mg_on();

    // 1. DRIVE TO FIRST LONG GOAL (mirror of elim_left — turns go positive/right)
    chassis.pid_drive_set(60_cm, 110); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(90, 90); chassis.pid_wait_until(85);
    chassis.pid_drive_set(75_cm, 110); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(70, 90); chassis.pid_wait();
    chassis.pid_drive_set(35_cm, 80); chassis.pid_wait();

    // 2. SCORE FIRST LONG GOAL
    intake_off();
    score_long_safe();

    // 3. MATCHLOAD (same as elim_left)
    chassis.pid_drive_set(-25_cm, 110); chassis.pid_wait_until(-5_cm);
    intake_on();
    chassis.pid_drive_set(45_cm, 110); chassis.pid_wait();
    matchload_on(); pros::delay(300);
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // 4. DRIVE TO SECOND LONG GOAL (turns go negative this time to reach right side)
    chassis.pid_drive_set(-45_cm, 110); chassis.pid_wait_until(-5_cm);
    chassis.pid_turn_set(-90, 90); chassis.pid_wait();
    chassis.pid_drive_set(35_cm, 80); chassis.pid_wait();

    // 5. SCORE SECOND LONG GOAL
    intake_off();
    score_long_safe();

    // 6. WING PUSH (mirror of elim_left — turn directions are flipped)
    chassis.pid_drive_set(-20_cm, 110); chassis.pid_wait_until(-5_cm);
    wings_on();
    chassis.pid_turn_set(-45, 90); chassis.pid_wait_until(-40);
    chassis.pid_drive_set(10_cm, 80); chassis.pid_wait_until(2_cm);
    chassis.pid_turn_set(45, 90); chassis.pid_wait_until(40);
    chassis.drive_set(80, 80);  // Constant voltage push for final shove
    pros::delay(250);
    chassis.drive_set(0, 0);
    intake_off();
}

void auton_awp_left() {
    wings_off(); matchload_off();
    intake_on(); mg_on();

    // 1. FIRST MATCHLOAD (mirror of awp_right — turns go positive/left)
    chassis.pid_drive_set(45_cm, 110); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(90, 90); chassis.pid_wait();
    matchload_on(); chassis.pid_drive_set(85_cm, 60); chassis.pid_wait();
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // 2. SCORE FIRST LONG GOAL
    chassis.pid_drive_set(-95_cm, 110); chassis.pid_wait_until(-3_cm);
    intake_off();
    score_long_safe();

    // 3. NAVIGATE TO LOW GOAL (slight path differences from awp_right)
    intake_on();
    chassis.pid_drive_set(10_cm, 80); chassis.pid_wait_until(2_cm);
    chassis.pid_turn_set(115, 90); chassis.pid_wait_until(110);
    chassis.pid_drive_set(30_cm, 110); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(25, 90); chassis.pid_wait_until(20);
    chassis.pid_drive_set(35_cm, 80); chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(35, 90); chassis.pid_wait();

    // 4. SCORE LOW GOAL
    // score_low runs the intake in reverse — blocks exit out the back into the low goal.
    // No intake_off needed beforehand because reverse intake IS the scoring action.
    score_low();

    // 5. SECOND MATCHLOAD (same as awp_right step 5)
    chassis.pid_drive_set(90_cm, 110); chassis.pid_wait_until(10_cm);
    chassis.pid_turn_set(40, 90); chassis.pid_wait();
    matchload_on(); chassis.pid_drive_set(85_cm, 60); chassis.pid_wait();
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // 6. SCORE SECOND LONG GOAL
    chassis.pid_drive_set(-95_cm, 110); chassis.pid_wait_until(-2_cm);
    intake_off();
    score_long_safe();
}

void auton_3plus4_left() {
    wings_off(); matchload_off();
    intake_on();
    mg_on();

    // 1. SWEEP THE FLOOR: Grab the 3 starting blocks
    // Drive forward while intake is running to collect field blocks as you move.
    // pid_wait_until(5_cm) exits the drive early so the turn starts immediately,
    // making the path smoother and faster than a full stop + turn.
    chassis.pid_drive_set(60_cm, 110);
    chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(-90, 90);
    chassis.pid_wait_until(-85);
    chassis.pid_drive_set(75_cm, 110);
    chassis.pid_wait();

    // 2. SCORE MIDDLE GOAL
    // Turn to face the middle goal, retract MG piston so it clears the opening,
    // drive in slowly, score, pull out 5cm to avoid dragging blocks back.
    chassis.pid_turn_set(35, 90);
    chassis.pid_wait();
    mg_off();
    intake_off();
    chassis.pid_drive_set(20_cm, 60); chassis.pid_wait();
    score_middle();
    chassis.pid_drive_set(5_cm, 60); chassis.pid_wait();
    mg_on();

    // 3. GO TO MATCHLOADER
    // Turn on intake, drive toward the matchloader, extend the piston, then drive
    // in slowly (60 speed) so triballs are pulled in gently. Wiggle sequence
    // ensures no balls are still stuck before retracting the piston.
    intake_on();
    chassis.pid_drive_set(-15_cm, 110); chassis.pid_wait_until(-5_cm);
    chassis.pid_turn_set(40, 90); chassis.pid_wait();
    matchload_on();
    chassis.pid_drive_set(95_cm, 60); chassis.pid_wait();
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // 4. FINAL WING SCORE (Long Goal)
    // Wings extend mid-approach to sweep field blocks toward the long goal.
    // Turn to 45° positions the robot to use the wings as a sweeping arm.
    // score_long_safe handles vision align + bumper contact + double-tap scoring.
    intake_off();
    chassis.pid_drive_set(-90_cm, 110); chassis.pid_wait_until(-10_cm);
    wings_on();
    chassis.pid_turn_set(-45, 90); chassis.pid_wait_until(-40);
    score_long_safe();
    wings_off();
}

void auton_3plus4_right() {
    wings_off(); matchload_off();
    intake_on();
    mg_on();

    // 1. SWEEP THE FLOOR: Grab the 3 starting blocks (mirror of 3+4 left)
    chassis.pid_drive_set(60_cm, 110);
    chassis.pid_wait_until(5_cm);
    chassis.pid_turn_set(90, 90);
    chassis.pid_wait_until(85);
    chassis.pid_drive_set(75_cm, 110);
    chassis.pid_wait();

    // 2. SCORE LOW GOAL
    // score_low runs intake in reverse so blocks exit out the back into the low goal.
    // 500ms delay gives enough time for blocks to fully exit before moving on.
    chassis.pid_turn_set(-35, 90);
    chassis.pid_wait();
    score_low();
    pros::delay(500);

    // 3. GO TO MATCHLOADER (mirror of 3+4 left — turn goes to -40 instead of +40)
    intake_on();
    chassis.pid_drive_set(-15_cm, 110); chassis.pid_wait_until(-5_cm);
    chassis.pid_turn_set(-40, 90); chassis.pid_wait();
    matchload_on();
    chassis.pid_drive_set(95_cm, 60); chassis.pid_wait();
    chassis.drive_set(30, 30); pros::delay(150);
    chassis.drive_set(-30, -30); pros::delay(150);
    chassis.drive_set(0, 0); pros::delay(200);
    matchload_off();

    // 4. FINAL WING SCORE (Long Goal) — mirror of 3+4 left, turn goes to +45
    intake_off();
    chassis.pid_drive_set(-90_cm, 110); chassis.pid_wait_until(-10_cm);
    wings_on();
    chassis.pid_turn_set(45, 90); chassis.pid_wait_until(40);
    score_long_safe();
    wings_off();
}
