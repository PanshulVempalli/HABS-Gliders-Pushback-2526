// ============================================================
//  Special thanks to:
//    - Jess Zarchi (ssejrog) for creating EZ-Template
//      https://github.com/EZ-Robotics/EZ-Template
//    - Purdue ACM SIGBots (BLRS) for creating PROS
//      https://pros.cs.purdue.edu/
/////
// For installation, upgrading, documentations, and tutorials, check out our website!
// https://ez-robotics.github.io/EZ-Template/
/////
// ============================================================
//  main.cpp — Robot initialization, screen UI, and driver control
//  Team: HABS Gliders 34071B
//  Team Member : Panshul Vempalli
//  Team Website : https://habs-gliders-34071b.vercel.app/
//  Contact Info : panshulvempalli@gmail.com
//  Game: VEX Push Back 2025/26
//
//  This file contains:
//    - Brain screen UI (LVGL background, banner, status overlay)
//    - initialize()         — runs once on startup
//    - autonomous()         — called by the field control system
//    - opcontrol()          — driver control loop (runs every ~10ms)
//    - ez_template_extras() — PID tuner + auton test shortcut
//
//  BUTTON MAP (opcontrol):
//    L1           — Intake forward (+ slow outtake barrier at 10%)
//    L2           — Intake reverse
//    Y            — Outtake forward (full power)
//    A            — Outtake reverse (full power)
//    R1           — Deploy wings
//    R2           — Retract wings
//    B            — Matchload macro (piston + intake + slow outtake)
//    X            — Stop matchload piston
//    LEFT arrow   — MG piston retract + intake
//    RIGHT arrow  — MG piston extend
//    UP / DOWN    — Straight-drive lock (both sides follow left stick)
// ============================================================

#include "main.h"


// ── Screen overlay ────────────────────────────────────────────────────────────
// These two pointers hold the LVGL objects for the status overlay panel and its
// text label. They're declared at file scope so show_status() and hide_status()
// can access them from anywhere in this file.
static lv_obj_t *status_overlay = nullptr;
static lv_obj_t *status_label   = nullptr;

// Writes text onto the status overlay and makes it visible.
// The nullptr guard prevents a crash if initialize() hasn't finished yet.
// lv_obj_move_foreground ensures it draws on top of the auton selector tiles.
static void show_status(const char *text) {
  if (!status_overlay) return;
  lv_label_set_text(status_label, text);
  lv_obj_clear_flag(status_overlay, LV_OBJ_FLAG_HIDDEN);
  lv_obj_move_foreground(status_overlay);
}

// Hides the status overlay so the auton selector is visible again.
// Called in disabled() so the screen returns to normal between runs.
static void hide_status() {
  if (!status_overlay) return;
  lv_obj_add_flag(status_overlay, LV_OBJ_FLAG_HIDDEN);
}

// Chassis constructor — tells EZ-Template which motor ports are on each side,
// which port the IMU is on, and the physical wheel size + RPM so it can
// calculate how far the robot has actually traveled during autonomous.
// Negative port numbers mean that motor is wired in reverse and needs to be
// flipped in software so all 6 drive motors push in the same direction.
ez::Drive chassis(
    // These are your drive motors, the first motor is used for sensing!
    {1, 2, 3},     // Left Chassis Ports (negative port will reverse it!)
    {-4, -5, -6},  // Right Chassis Ports (negative port will reverse it!)

    7,      // IMU Port
    4.125,  // Wheel Diameter (Remember, 4" wheels without screw holes are actually 4.125!)
    343);   // Wheel RPM = cartridge * (motor gear / wheel gear)

// Uncomment the trackers you're using here!
// - `8` and `9` are smart ports (making these negative will reverse the sensor)
//  - you should get positive values on the encoders going FORWARD and RIGHT
// - `2.75` is the wheel diameter
// - `4.0` is the distance from the center of the wheel to the center of the robot
// ez::tracking_wheel horiz_tracker(8, 2.75, 4.0);  // This tracking wheel is perpendicular to the drive wheels
// ez::tracking_wheel vert_tracker(9, 2.75, 4.0);   // This tracking wheel is parallel to the drive wheels

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
  // Print our branding over your terminal :D
  ez::ez_template_print();

  // Wait 500ms before touching anything — legacy ADI ports (pneumatics, encoders)
  // need a moment to power up, and reading them too early can produce bad values.
  pros::delay(500);

  // Look at your horizontal tracking wheel and decide if it's in front of the midline of your robot or behind it
  //  - change `back` to `front` if the tracking wheel is in front of the midline
  //  - ignore this if you aren't using a horizontal tracker
  // chassis.odom_tracker_back_set(&horiz_tracker);
  // Look at your vertical tracking wheel and decide if it's to the left or right of the center of the robot
  //  - change `left` to `right` if the tracking wheel is to the right of the centerline
  //  - ignore this if you aren't using a vertical tracker
  // chassis.odom_tracker_left_set(&vert_tracker);

  // curve_buttons_toggle(true) lets the driver press joystick buttons to adjust
  // the response curve live during a match, which makes slow movements smoother.
  // activebrake at 0 means the chassis freewheels when the stick is at zero —
  // set to ~2 if you want the robot to actively hold position when released.
  // curve_default_set(0, 0) applies no curve until the driver changes it;
  // comment this line out if you have an SD card so saved values load instead.
  chassis.opcontrol_curve_buttons_toggle(true);
  chassis.opcontrol_drive_activebrake_set(0.0);
  chassis.opcontrol_curve_default_set(0.0, 0.0);

  // Load the PID / motion constants defined in autons.cpp so autonomous
  // movements use our tuned values rather than EZ-Template defaults.
  default_constants();

  // These are already defaulted to these buttons, but you can change the left/right curve buttons here!
  // chassis.opcontrol_curve_buttons_left_set(pros::E_CONTROLLER_DIGITAL_LEFT, pros::E_CONTROLLER_DIGITAL_RIGHT);  // If using tank, only the left side is used.
  // chassis.opcontrol_curve_buttons_right_set(pros::E_CONTROLLER_DIGITAL_Y, pros::E_CONTROLLER_DIGITAL_A);

  // Register every autonomous routine with the selector so the driver can
  // scroll through them on the brain screen before a match and pick one.
  // Each entry is {display name shown on screen, function to call}.
  ez::as::auton_selector.autons_add({
      {"Drive\n\nDrive forward and come back", drive_example},
      {"Turn\n\nTurn 3 times.", turn_example},
      {"Drive and Turn\n\nDrive forward, turn, come back", drive_and_turn},
      {"Drive and Turn\n\nSlow down during drive", wait_until_change_speed},
      {"Swing Turn\n\nSwing in an 'S' curve", swing_example},
      {"Motion Chaining\n\nDrive forward, turn, and come back, but blend everything together :D", motion_chaining},
      {"Combine all 3 movements", combining_movements},
      {"Interference\n\nAfter driving forward, robot performs differently if interfered or not", interfered_example},
      {"Simple Odom\n\nThis is the same as the drive example, but it uses odom instead!", odom_drive_example},
      {"Pure Pursuit\n\nGo to (0, 30) and pass through (6, 10) on the way.  Come back to (0, 0)", odom_pure_pursuit_example},
      {"Pure Pursuit Wait Until\n\nGo to (24, 24) but start running an intake once the robot passes (12, 24)", odom_pure_pursuit_wait_until_example},
      {"Boomerang\n\nGo to (0, 24, 45) then come back to (0, 0, 0)", odom_boomerang_example},
      {"Boomerang Pure Pursuit\n\nGo to (0, 24, 45) on the way to (24, 24) then come back to (0, 0, 0)", odom_boomerang_injected_pure_pursuit_example},
      {"Measure Offsets\n\nThis will turn the robot a bunch of times and calculate your offsets for your tracking wheels.", measure_offsets},
      {"AWP Right\n\nRight side AWP autonomous", auton_awp_right},
      {"AWP Left\n\nLeft side AWP autonomous", auton_awp_left},
      {"Elim Right\n\nRight side elimination autonomous", auton_elim_right},
      {"Elim Left\n\nLeft side elimination autonomous", auton_elim_left},
      {"3+4 Left\n\nSweep 3 blocks, score middle, matchload, wing long goal (left)", auton_3plus4_left},
      {"3+4 Right\n\nSweep 3 blocks, score low, matchload, wing long goal (right)", auton_3plus4_right},
      {"Prog Skills\n\nFirst half: intake floor blocks, matchload, score long goal cycle", auton_prog_skills},
  });

  // chassis.initialize() calibrates the IMU and starts EZ-Template background tasks.
  // ez::as::initialize() draws the auton selector on the brain screen.
  // The controller rumble tells the driver whether the IMU calibrated successfully:
  //   single dot "."  = IMU good, robot is ready to compete
  //   three dashes "---" = IMU failed — restart the robot before a match or
  //                         all autonomous turns will be wrong
  chassis.initialize();
  ez::as::initialize();
  master.rumble(chassis.drive_imu_calibrated() ? "." : "---");

  // ── Brain screen UI ───────────────────────────────────────────────────────
  // Build a dark background panel first so it sits behind every other element.
  // 480x240 is the full resolution of the V5 brain screen.
  lv_obj_t *bg = lv_obj_create(lv_scr_act());
  lv_obj_set_size(bg, 480, 240);
  lv_obj_set_pos(bg, 0, 0);
  lv_obj_set_style_bg_color(bg, lv_color_hex(0x1a1a2e), LV_PART_MAIN);  // dark navy
  lv_obj_set_style_border_width(bg, 0, LV_PART_MAIN);  // remove default border line
  lv_obj_set_style_radius(bg, 0, LV_PART_MAIN);        // square corners, not rounded
  lv_obj_clear_flag(bg, LV_OBJ_FLAG_SCROLLABLE);       // prevent accidental scroll on touch

  // Red banner strip across the top 40px — used as a team colour identifier.
  lv_obj_t *banner = lv_obj_create(lv_scr_act());
  lv_obj_set_size(banner, 480, 40);
  lv_obj_set_pos(banner, 0, 0);
  lv_obj_set_style_bg_color(banner, lv_color_hex(0xe94560), LV_PART_MAIN);  // team red
  lv_obj_set_style_border_width(banner, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(banner, 0, LV_PART_MAIN);
  lv_obj_clear_flag(banner, LV_OBJ_FLAG_SCROLLABLE);

  // White team name text centred inside the red banner.
  lv_obj_t *team_label = lv_label_create(banner);
  lv_label_set_text(team_label, "VEX ROBOTICS  |  EZ-Template");
  lv_obj_set_style_text_color(team_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_align(team_label, LV_ALIGN_CENTER, 0, 0);

  // ── Status overlay ────────────────────────────────────────────────────────
  // Full-screen semi-transparent panel that covers the auton selector during a run
  // so the driver can see at a glance whether the robot is in auton or driver mode.
  // It starts hidden (LV_OBJ_FLAG_HIDDEN) and is revealed by show_status() when
  // a run begins, then hidden again by hide_status() when the robot is disabled.
  status_overlay = lv_obj_create(lv_scr_act());
  lv_obj_set_size(status_overlay, 480, 240);
  lv_obj_set_pos(status_overlay, 0, 0);
  lv_obj_set_style_bg_color(status_overlay, lv_color_hex(0x1a1a2e), LV_PART_MAIN);
  lv_obj_set_style_bg_opa(status_overlay, LV_OPA_90, LV_PART_MAIN);  // 90% opaque — background bleeds through slightly
  lv_obj_set_style_border_width(status_overlay, 0, LV_PART_MAIN);
  lv_obj_set_style_radius(status_overlay, 0, LV_PART_MAIN);
  lv_obj_clear_flag(status_overlay, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_add_flag(status_overlay, LV_OBJ_FLAG_HIDDEN);  // hidden until auton or driver period starts

  // Large white text centred in the overlay — updated by show_status() each run.
  status_label = lv_label_create(status_overlay);
  lv_label_set_text(status_label, "");
  lv_obj_set_style_text_color(status_label, lv_color_hex(0xffffff), LV_PART_MAIN);
  lv_obj_set_style_text_font(status_label, &lv_font_montserrat_24, LV_PART_MAIN);
  lv_obj_align(status_label, LV_ALIGN_CENTER, 0, 0);
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {
  // Hide the status overlay so the auton selector is visible again between runs.
  hide_status();
}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {
  // . . .
}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
  show_status("Auton in progress...");

  // Reset all chassis state so every auton run starts from a clean baseline.
  // Without these resets, leftover sensor values from a previous run could make
  // the robot drive the wrong distance or turn to the wrong angle.
  chassis.pid_targets_reset();   // clear any leftover PID target from the last run
  chassis.drive_imu_reset();     // zero the heading so turns are relative to start angle
  chassis.drive_sensor_reset();  // zero drive encoders so distance is measured from 0
  chassis.odom_xyt_set(0_in, 0_in, 0_deg);  // place robot at (0,0) facing 0° in odom space
  chassis.drive_brake_set(MOTOR_BRAKE_HOLD); // hold position after each move instead of coasting

  // Call whichever auton the driver selected on the brain screen before the match.
  ez::as::auton_selector.selected_auton_call();

  show_status("Auton Finished!");
}

/**
 * Simplifies printing tracker values to the brain screen
 */
void screen_print_tracker(ez::tracking_wheel *tracker, std::string name, int line) {
  std::string tracker_value = "", tracker_width = "";
  // Only read from the tracker if one is actually plugged in — a nullptr means
  // no tracker is configured for that side, so skip it to avoid a crash.
  if (tracker != nullptr) {
    tracker_value = name + " tracker: " + util::to_string_with_precision(tracker->get());             // Make text for the tracker value
    tracker_width = "  width: " + util::to_string_with_precision(tracker->distance_to_center_get());  // Make text for the distance to center
  }
  ez::screen_print(tracker_value + tracker_width, line);  // Print final tracker text
}

/**
 * Ez screen task
 * Adding new pages here will let you view them during user control or autonomous
 * and will help you debug problems you're having
 */
void ez_screen_task() {
  while (true) {
    // Skip debug pages at a competition — we don't want them cluttering the screen
    // during a match or confusing the driver.
    if (!pros::competition::is_connected()) {
      // Only show the odom debug page when odom is enabled and the PID tuner
      // isn't open (they'd conflict for the same screen space).
      if (chassis.odom_enabled() && !chassis.pid_tuner_enabled()) {
        // page_blank_is_on(0) checks if the driver scrolled to the first blank
        // debug page on the brain screen — only draw when it's active.
        if (ez::as::page_blank_is_on(0)) {
          // Print X, Y position and heading (theta) so you can watch the robot's
          // estimated position update in real time during driving or auton testing.
          ez::screen_print("x: " + util::to_string_with_precision(chassis.odom_x_get()) +
                               "\ny: " + util::to_string_with_precision(chassis.odom_y_get()) +
                               "\na: " + util::to_string_with_precision(chassis.odom_theta_get()),
                           1);  // Don't override the top Page line

          // Print each tracking wheel's current reading and its offset from the
          // robot's centre — useful for diagnosing odom drift between runs.
          screen_print_tracker(chassis.odom_tracker_left, "l", 4);
          screen_print_tracker(chassis.odom_tracker_right, "r", 5);
          screen_print_tracker(chassis.odom_tracker_back, "b", 6);
          screen_print_tracker(chassis.odom_tracker_front, "f", 7);
        }
      }
    }

    // At a competition, remove all blank debug pages so only the auton selector
    // and normal EZ-Template pages remain visible to the driver.
    else {
      if (ez::as::page_blank_amount() > 0)
        ez::as::page_blank_remove_all();
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
// Launch the screen task as a permanent background task so it runs in parallel
// with opcontrol and autonomous without blocking either of them.
pros::Task ezScreenTask(ez_screen_task);

/**
 * Gives you some extras to run in your opcontrol:
 * - run your autonomous routine in opcontrol by pressing DOWN and B
 *   - to prevent this from accidentally happening at a competition, this
 *     is only enabled when you're not connected to competition control.
 * - gives you a GUI to change your PID values live by pressing X
 */
void ez_template_extras() {
  // Only run this when not connected to a competition switch
  if (!pros::competition::is_connected()) {
    // PID Tuner
    // - after you find values that you're happy with, you'll have to set them in auton.cpp

    // X toggles the PID tuner overlay on the brain screen.
    // get_digital_new_press only fires on the first frame the button is pressed,
    // not every frame it's held — so one press = one toggle, no rapid flickering.
    if (master.get_digital_new_press(DIGITAL_X))
      chassis.pid_tuner_toggle();

    // B + DOWN re-runs the selected auton while in driver control — useful for
    // testing without switching to auton mode. The brake mode is saved and restored
    // so the robot doesn't stay in HOLD mode permanently after the test run.
    if (master.get_digital(DIGITAL_B) && master.get_digital(DIGITAL_DOWN)) {
      pros::motor_brake_mode_e_t preference = chassis.drive_brake_get();
      autonomous();
      chassis.drive_brake_set(preference);
    }

    // Must be called every loop iteration when the tuner is open so it can
    // process button presses and update the displayed constants.
    chassis.pid_tuner_iterate();
  }

  // If we're at a competition and the tuner somehow got left on, disable it so
  // it doesn't interfere with the auton selector display.
  else {
    if (chassis.pid_tuner_enabled())
      chassis.pid_tuner_disable();
  }
}

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */

 // - Driver Code ----------------------------------
void opcontrol() {
  // COAST means drive motors spin freely when the stick is at zero.
  // This feels more natural for a driver than HOLD, which would fight every push
  // from a defender and make the robot harder to maneuver.
  chassis.drive_brake_set(MOTOR_BRAKE_COAST);

  // This loop runs every ~10ms (DELAY_TIME) for the full 1:45 driver period.
  // Every subsystem is polled each iteration so no button press is ever missed.
  while (true) {

    // 1. DRIVE CONTROL
    // Holding UP or DOWN locks both sides of the drivetrain to the same stick value
    // so the robot drives perfectly straight without the driver having to balance
    // two sticks manually. Without this macro, tiny differences between the left
    // and right stick positions cause the robot to drift even on straight runs.
    // When neither button is held, normal tank drive runs: left stick = left wheels,
    // right stick = right wheels, fully independent.
    if (master.get_digital(DIGITAL_UP)) {
      int forward_power = master.get_analog(ANALOG_LEFT_Y);
      chassis.drive_set(forward_power, forward_power);
    }
    else if (master.get_digital(DIGITAL_DOWN)) {
      int backward_power = master.get_analog(ANALOG_LEFT_Y);
      chassis.drive_set(backward_power, backward_power);
    }
    else {
      chassis.opcontrol_tank();
    }

    // 2. INTAKE CONTROL (L1 Forward, L2 Reverse)
    // When intaking forward (L1), the outtake also spins at 10% (1200/12000 mV).
    // This slow reverse spin acts as a soft barrier — it keeps scored blocks from
    // rolling back down out of the uptake zone while the intake is pulling new
    // blocks in. Without it, blocks can slip back through when the intake stops.
    if (master.get_digital(DIGITAL_L1)) {
      intake.move_voltage(12000);
      outtake.move_voltage(1200);  // 10% — holds blocks in uptake zone
    } else if (master.get_digital(DIGITAL_L2)) {
      intake.move_voltage(-12000);
    } else {
      // Don't brake the intake here if B or LEFT is being held — those macros
      // also run the intake and would get cut off mid-cycle if we braked here.
      if (!master.get_digital(DIGITAL_B) && !master.get_digital(DIGITAL_LEFT)) {
        intake.brake();
      }
    }

    // 3. OUTTAKE CONTROL (Y Forward, A Reverse)
    // Y and A give full manual outtake control and override everything else.
    // When neither is pressed, only brake if no intake-forward button (L1/B/LEFT)
    // is active — otherwise this block would immediately cancel the slow 10% spin
    // that was just set in the intake or macro sections above.
    if (master.get_digital(DIGITAL_Y)) {
      outtake.move_voltage(12000);
    } else if (master.get_digital(DIGITAL_A)) {
      outtake.move_voltage(-12000);
    } else if (!master.get_digital(DIGITAL_L1) &&
               !master.get_digital(DIGITAL_B) &&
               !master.get_digital(DIGITAL_LEFT)) {
      outtake.brake();
    }

    // 4. WING PISTON (R1 Deploy, R2 Retract)
    // Pistons are digital on/off — true = extended, false = retracted.
    // No else-brake is needed because pneumatics hold their position without power.
    if (master.get_digital(DIGITAL_R1)) {
      wing_piston.set_value(true);
    } else if (master.get_digital(DIGITAL_R2)) {
      wing_piston.set_value(false);
    }

    // 5. MATCHLOADING MACRO (B Start, X Stop)
    // B extends the match loader piston to catch triballs AND runs the intake so
    // triballs are immediately pulled into the robot as they're loaded — the driver
    // doesn't need to press a separate intake button during matchloading.
    // The slow outtake (same as L1) keeps already-scored blocks held in the uptake
    // zone so they don't fall back down while new ones are being fed in.
    // X retracts the piston to stop accepting triballs.
    if (master.get_digital(DIGITAL_B)) {
      match_loader_piston.set_value(true);
      intake.move_voltage(12000);
      outtake.move_voltage(1200);  // 10% — holds blocks in uptake zone
    }
    if (master.get_digital(DIGITAL_X)) {
      match_loader_piston.set_value(false);
    }

    // 6. MG PISTON MACRO (Left Arrow: Retract+Intake | Right Arrow: Extend)
    // LEFT retracts the MG piston and simultaneously runs the intake so the robot
    // can sweep in field blocks as the piston pulls back out of the way — combining
    // both actions into one button press saves reaction time during a run.
    // The slow outtake (same as L1) keeps blocks held in the uptake zone while
    // the intake sweeps new ones in from the field.
    // RIGHT extends the MG piston to push blocks or set up for a scoring cycle.
    if (master.get_digital(DIGITAL_LEFT)) {
      mg_piston.set_value(false);
      intake.move_voltage(12000);
      outtake.move_voltage(1200);  // 10% — holds blocks in uptake zone
    } else if (master.get_digital(DIGITAL_RIGHT)) {
      mg_piston.set_value(true);
    }

    pros::delay(ez::util::DELAY_TIME);
  }
}
