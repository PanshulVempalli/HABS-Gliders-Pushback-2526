#include "vex.h"
#include "robot-config.h"
#include <string>
#include <vector>
#include <cmath>

using namespace vex;

// ---------------- IMU ----------------
inertial imu = inertial(PORT7);

// ---------------- DRIVE MOTORS ----------------
motor left_front = motor(PORT1, ratio18_1, false);
motor left_middle = motor(PORT2, ratio18_1, false);
motor left_back = motor(PORT3, ratio6_1, false);

motor right_front = motor(PORT4, ratio18_1, true);
motor right_middle = motor(PORT5, ratio18_1, true);
motor right_back = motor(PORT6, ratio6_1, true);

// (Using motor_groups for the drive_motors list logic)
motor_group left_drive = motor_group(left_front, left_middle, left_back);
motor_group right_drive = motor_group(right_front, right_middle, right_back);
motor_group all_drive = motor_group(left_front, left_middle, left_back, right_front, right_middle, right_back);

// ---------------- MECHANISMS ----------------
motor intake = motor(PORT10, ratio18_1, false);
motor outtake = motor(PORT8, ratio18_1, false);

// ---------------- SENSORS ----------------
bumper back_bumper = bumper(Brain.ThreeWirePort.H);
vision vision_sensor = vision(PORT9);

// ---------------- PNEUMATICS ----------------
digital_out match_loader_piston = digital_out(Brain.ThreeWirePort.A);
digital_out wing_piston = digital_out(Brain.ThreeWirePort.B);
digital_out mg_piston = digital_out(Brain.ThreeWirePort.C);

// ---------------- WHEEL CONSTANTS ----------------
const double WHEEL_DIAMETER_CM = 8.255;
const double WHEEL_CIRCUMFERENCE = WHEEL_DIAMETER_CM * 3.1416;

// ---------------- GLOBAL ----------------
std::string selected_auton = "RIGHT_ELIM";  // fallback if nothing confirmed
bool confirmed = false;
bool confirming = false;  // true = showing confirmation screen

// Navy blue to match team branding
const vex::color NAVY  = vex::color(15, 25, 60);
const vex::color RED   = vex::color(210, 0, 0);
const vex::color DIM   = vex::color(40, 50, 90);   // unselected button
const vex::color HILIT = vex::color(60, 5, 5);     // selected button tint

// ---------------- HELPERS ----------------
void stop_drive() {
    all_drive.stop();
}

void reset_drive() {
    all_drive.setPosition(0, degrees);
}

double motor_position() {
    return all_drive.position(degrees);
}

double get_distance_cm() {
    return (motor_position() / 360.0) * WHEEL_CIRCUMFERENCE;
}

// ---------------- VISION ----------------
void vision_align(double strength = 0.3, int threshold = 10) {
    vision_sensor.takeSnapshot(1);
    if (vision_sensor.largestObject.exists) {
        int error = vision_sensor.largestObject.centerX - 158;

        if (std::abs(error) < threshold) {
            return;
        }

        double turn_power = error * strength;

        left_drive.spin(forward, turn_power, percent);
        right_drive.spin(reverse, turn_power, percent);

        wait(200, msec);
        stop_drive();
    }
}

// ---------------- CHASSIS ----------------
class Chassis {
public:
    double kp_drive = 0.6;
    double kd_drive = 0.12;
    double kp_turn = 0.8;
    double kd_turn = 0.25;

    void move_distance(double target_cm) {
        reset_drive();
        double start_heading = imu.rotation(degrees);

        double error = target_cm - get_distance_cm();
        double prev_error = error;

        while (std::abs(error) > 1) {
            double current = get_distance_cm();
            error = target_cm - current;
            double derivative = error - prev_error;

            double power = (kp_drive * error) + (kd_drive * derivative);

            if (std::abs(power) < 5) {
                power = (power > 0) ? 5 : -5;
            }

            double heading_error = imu.rotation(degrees) - start_heading;
            double correction = heading_error * 0.6;

            left_drive.spin(forward, power - correction, percent);
            right_drive.spin(forward, power + correction, percent);

            prev_error = error;
            wait(20, msec);
        }
        stop_drive();
    }

    void turn_to_angle(double target) {
        double error = target - imu.rotation(degrees);
        double prev_error = error;

        while (std::abs(error) > 1) {
            error = target - imu.rotation(degrees);
            double derivative = error - prev_error;

            double power = (kp_turn * error) + (kd_turn * derivative);

            if (std::abs(power) < 5) {
                power = (power > 0) ? 5 : -5;
            }

            left_drive.spin(forward, power, percent);
            right_drive.spin(reverse, power, percent);

            prev_error = error;
            wait(20, msec);
        }
        stop_drive();
    }
};

Chassis my_chassis;

// ---------------- BUMPER SAFE ----------------
void score_long_safe() {
    vision_align();

    int timeout = 1200;
    int timer = 0;

    while (!back_bumper.pressing() && timer < timeout) {
        all_drive.spin(reverse, 40, percent);
        wait(20, msec);
        timer += 20;
    }

    stop_drive();

    if (!back_bumper.pressing()) {
        my_chassis.turn_to_angle(imu.rotation(degrees) - 10);

        timer = 0;
        while (!back_bumper.pressing() && timer < timeout) {
            all_drive.spin(reverse, 40, percent);
            wait(20, msec);
            timer += 20;
        }

        stop_drive();
    }

    wait(100, msec);
    // score_long() called below
}

// ---------------- INTAKE ----------------
void intake_on() {
    intake.spin(forward, 100, percent);
}

void intake_off() {
    intake.stop();
}

// ---------------- SCORING ----------------
void score_middle() {
    outtake.spin(forward, 100, percent);
    wait(0.5, seconds);
    outtake.stop();
}

void score_low() {
    intake.spin(reverse, 100, percent);
}

void score_long() {
    outtake.spin(reverse, 100, percent);
    wait(1.0, seconds);
    outtake.stop();
}

// ---------------- PNEUMATICS ----------------
void matchload_on() {
    match_loader_piston.set(true);
}

void matchload_off() {
    match_loader_piston.set(false);
}

void wings_on() {
    wing_piston.set(true);
}

void wings_off() {
    wing_piston.set(false);
}

void mg_on() {
    mg_piston.set(true);
}

void mg_off() {
    mg_piston.set(false);
}

// helper: auton key → display name
const char* auton_display_name(std::string key) {
    if (key == "RIGHT_AWP")  return "RIGHT AWP";
    if (key == "LEFT_AWP")   return "LEFT AWP";
    if (key == "LEFT_ELIM")  return "LEFT ELIM";
    if (key == "RIGHT_ELIM") return "RIGHT ELIM";
    return "NONE";
}

// ---------------- SELECTION SCREEN ----------------
void draw_selection() {
    Brain.Screen.setFillColor(NAVY);
    Brain.Screen.setPenColor(NAVY);
    Brain.Screen.drawRectangle(0, 0, 480, 240);

    // Title
    Brain.Screen.setPenColor(RED);
    Brain.Screen.setFont(prop20);
    Brain.Screen.printAt(155, 22, "SELECT AUTONOMOUS");

    // Draw one auton button
    struct BtnDef { int x; int y; const char* label; const char* key; };
    BtnDef buttons[4] = {
        {10,  35, "RIGHT AWP",  "RIGHT_AWP"},
        {250, 35, "LEFT AWP",   "LEFT_AWP"},
        {10,  135,"LEFT ELIM",  "LEFT_ELIM"},
        {250, 135,"RIGHT ELIM", "RIGHT_ELIM"},
    };

    for (int i = 0; i < 4; i++) {
        bool sel = (selected_auton == buttons[i].key);
        Brain.Screen.setFillColor(sel ? HILIT : DIM);
        Brain.Screen.setPenColor(sel ? HILIT : DIM);
        Brain.Screen.drawRectangle(buttons[i].x, buttons[i].y, 220, 80);
        Brain.Screen.setPenColor(RED);
        Brain.Screen.setFont(prop30);
        Brain.Screen.printAt(buttons[i].x + 12, buttons[i].y + 48, buttons[i].label);
    }
}

// ---------------- CONFIRMATION SCREEN ----------------
void draw_confirmation() {
    Brain.Screen.setFillColor(NAVY);
    Brain.Screen.setPenColor(NAVY);
    Brain.Screen.drawRectangle(0, 0, 480, 240);

    Brain.Screen.setPenColor(RED);
    Brain.Screen.setFont(prop20);
    Brain.Screen.printAt(120, 40, "Confirm you want to run:");

    Brain.Screen.setFont(prop40);
    Brain.Screen.printAt(100, 100, auton_display_name(selected_auton));

    // CONFIRM button
    Brain.Screen.setFillColor(DIM);
    Brain.Screen.setPenColor(DIM);
    Brain.Screen.drawRectangle(50, 155, 170, 60);
    Brain.Screen.setPenColor(RED);
    Brain.Screen.setFont(prop30);
    Brain.Screen.printAt(80, 193, "CONFIRM");

    // BACK button
    Brain.Screen.setFillColor(DIM);
    Brain.Screen.setPenColor(DIM);
    Brain.Screen.drawRectangle(260, 155, 170, 60);
    Brain.Screen.setPenColor(RED);
    Brain.Screen.setFont(prop30);
    Brain.Screen.printAt(310, 193, "BACK");
}

// ---------------- LOCKED SCREEN ----------------
void draw_locked() {
    Brain.Screen.setFillColor(NAVY);
    Brain.Screen.setPenColor(NAVY);
    Brain.Screen.drawRectangle(0, 0, 480, 240);

    Brain.Screen.setPenColor(RED);
    Brain.Screen.setFont(prop30);
    Brain.Screen.printAt(150, 90, "LOCKED IN:");

    Brain.Screen.setFont(prop40);
    Brain.Screen.printAt(100, 150, auton_display_name(selected_auton));
}

// ---------------- TOUCH ----------------
void screen_pressed() {
    if (confirmed) return;

    int x = Brain.Screen.xPosition();
    int y = Brain.Screen.yPosition();

    if (!confirming) {
        // Selection screen touch zones
        if      (x > 10  && x < 230 && y > 35  && y < 115) { selected_auton = "RIGHT_AWP";  confirming = true; }
        else if (x > 250 && x < 470 && y > 35  && y < 115) { selected_auton = "LEFT_AWP";   confirming = true; }
        else if (x > 10  && x < 230 && y > 135 && y < 215) { selected_auton = "LEFT_ELIM";  confirming = true; }
        else if (x > 250 && x < 470 && y > 135 && y < 215) { selected_auton = "RIGHT_ELIM"; confirming = true; }

        if (confirming) draw_confirmation();
        else            draw_selection();
    } else {
        // Confirmation screen touch zones
        if (x > 50 && x < 220 && y > 155 && y < 215) {
            // CONFIRM pressed
            confirmed = true;
            draw_locked();
        } else if (x > 260 && x < 430 && y > 155 && y < 215) {
            // BACK pressed
            confirming = false;
            selected_auton = "";
            draw_selection();
        }
    }
}

// ---------------- PRE AUTON ----------------
void pre_auton() {
    imu.calibrate();
    while (imu.isCalibrating()) {
        wait(100, msec);
    }

    draw_selection();
    Brain.Screen.pressed(screen_pressed);

    while (!confirmed) {
        wait(50, msec);
    }
}

// ---------------- AUTON ----------------

void auton_elim_left() { //----------------------------------
    intake_on();
    mg_on();
    // STEP 1: SWEEP + SCORE (≈ 1 tile forward sweep)
    my_chassis.move_distance(60); // 1 tile forward
    my_chassis.turn_to_angle(-90); // slight angle to collect
    my_chassis.move_distance(75); // finish sweep

    // ALIGN TO LONG GOAL
    my_chassis.turn_to_angle(-70);
    my_chassis.move_distance(15); // approach goal
    vision_align(0.2);
    score_long();

    // STEP 2: REVERSE OUT
    my_chassis.move_distance(-25);

    // STEP 3: GO TO MATCH LOADER (~0.75 tile sideways)
    my_chassis.move_distance(45);
    matchload_on();
    wait(0.5, seconds);
    matchload_off();

    // STEP 4: RETURN + SCORE
    my_chassis.move_distance(-45);
    my_chassis.turn_to_angle(-90);
    my_chassis.move_distance(15);
    vision_align(0.2);
    score_long();

    // STEP 5: MOVE TO SIDE OF GOAL (parallel setup)
    my_chassis.move_distance(-20);
    wings_on();
    my_chassis.turn_to_angle(45);
    my_chassis.move_distance(10);
    my_chassis.turn_to_angle(-45);

    // STEP 6: WING PUSH (≈ half tile push)
    my_chassis.move_distance(30);
    wait(0.3, seconds);
    intake_off();
}

void auton_awp_right() { //----------------------------------
    intake_on();

    // STEP 1: start → approach right match loader (~0.75 tile)
    my_chassis.move_distance(45);

    // STEP 2: turn into match loader
    my_chassis.turn_to_angle(-90);
    matchload_on();
    my_chassis.move_distance(18); // into loader
    wait(0.5, seconds);
    matchload_off();

    // STEP 3: reverse → long goal (~20 cm back)
    my_chassis.move_distance(-22);
    score_long();

    // STEP 4: small forward reset
    my_chassis.move_distance(10);

    // STEP 5: rotate toward center (~115°)
    my_chassis.turn_to_angle(-115);

    // move toward center line (~half tile)
    my_chassis.move_distance(30);

    // STEP 6: intake sweep angle (~+25° adjustment)
    my_chassis.turn_to_angle(25);
    my_chassis.move_distance(35); // collect 3 center blocks

    // STEP 7: align to middle goal
    my_chassis.turn_to_angle(35);

    // STEP 8: score middle goal
    mg_off();
    my_chassis.move_distance(10);
    score_middle();
    mg_on();

    // STEP 9: CROSS FIELD (this is key)
    // from right → left ≈ 1.5 tiles
    my_chassis.move_distance(90);

    // STEP 10: align to left match loader
    my_chassis.turn_to_angle(40);
    matchload_on();
    my_chassis.move_distance(20);
    wait(0.5, seconds);
    matchload_off();

    // STEP 11: reverse into long goal
    my_chassis.move_distance(-22);
    score_long();
    intake_off();
}

void auton_elim_right() { //----------------------------------
    intake_on();
    mg_on();

    // STEP 1: SWEEP + SCORE (≈ 1 tile forward sweep)
    my_chassis.move_distance(60); // 1 tile forward
    my_chassis.turn_to_angle(90); // slight angle to collect
    my_chassis.move_distance(75); // finish sweep

    // ALIGN TO LONG GOAL
    my_chassis.turn_to_angle(70);
    my_chassis.move_distance(15); // approach goal
    vision_align(0.2);
    score_long();

    // STEP 2: REVERSE OUT
    my_chassis.move_distance(-25);

    // STEP 3: GO TO MATCH LOADER (~0.75 tile sideways)
    my_chassis.move_distance(45);
    matchload_on();
    wait(0.5, seconds);
    matchload_off();

    // STEP 4: RETURN + SCORE
    my_chassis.move_distance(-45);
    my_chassis.turn_to_angle(-90);
    my_chassis.move_distance(15);
    vision_align(0.2);
    score_long();

    // STEP 5: MOVE TO SIDE OF GOAL (parallel setup)
    my_chassis.move_distance(-20);
    wings_on();
    my_chassis.turn_to_angle(-45);
    my_chassis.move_distance(10);
    my_chassis.turn_to_angle(45);

    // STEP 6: WING PUSH (≈ half tile push)
    my_chassis.move_distance(30);
    wait(0.3, seconds);
    intake_off();
}

void auton_awp_left() { //----------------------------------
    intake_on();

    // STEP 1: start → approach right match loader (~0.75 tile)
    my_chassis.move_distance(45);

    // STEP 2: turn into match loader
    my_chassis.turn_to_angle(90);
    matchload_on();
    my_chassis.move_distance(18); // into loader
    wait(0.5, seconds);
    matchload_off();

    // STEP 3: reverse → long goal (~20 cm back)
    my_chassis.move_distance(-22);
    score_long();

    // STEP 4: small forward reset
    my_chassis.move_distance(10);

    // STEP 5: rotate toward center (~115°)
    my_chassis.turn_to_angle(115);

    // move toward center line (~half tile)
    my_chassis.move_distance(30);

    // STEP 6: intake sweep angle (~+25° adjustment)
    my_chassis.turn_to_angle(25);
    my_chassis.move_distance(35); // collect 3 center blocks

    // STEP 7: align to middle goal
    my_chassis.turn_to_angle(35);

    // STEP 8: score low goal
    score_low();

    // STEP 9: CROSS FIELD (this is key)
    // from right → left ≈ 1.5 tiles
    my_chassis.move_distance(90);

    // STEP 10: align to left match loader
    my_chassis.turn_to_angle(40);
    matchload_on();
    my_chassis.move_distance(20);
    wait(0.5, seconds);
    matchload_off();

    // STEP 11: reverse into long goal
    my_chassis.move_distance(-22);
    score_long();
    intake_off();
}

// ---------------- RUN ----------------
void autonomous() {
    if (selected_auton == "RIGHT_AWP") {
        auton_awp_right();
    } else if (selected_auton == "LEFT_AWP") {
        auton_awp_left();
    } else if (selected_auton == "LEFT_ELIM") {
        auton_elim_left();
    } else if (selected_auton == "RIGHT_ELIM") {
        auton_elim_right();
    }
}

void usercontrol() {
    while (true) {
        // Add driver control code here
        wait(20, msec);
    }
}