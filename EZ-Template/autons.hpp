#pragma once

void default_constants();

void drive_example();
void turn_example();
void drive_and_turn();
void wait_until_change_speed();
void swing_example();
void motion_chaining();
void combining_movements();
void interfered_example();
void odom_drive_example();
void odom_pure_pursuit_example();
void odom_pure_pursuit_wait_until_example();
void odom_boomerang_example();
void odom_boomerang_injected_pure_pursuit_example();
void measure_offsets();

void auton_awp_right();
void auton_awp_left();
void auton_elim_right();
void auton_elim_left();
void auton_3plus4_left();
void auton_3plus4_right();
void auton_prog_skills();

void intake_on();
void intake_off();
void score_middle();
void score_low();
void score_long();
void mg_on();
void mg_off();
void wings_on();
void wings_off();
void matchload_on();
void matchload_off();
void vision_align(double strength = 0.3, int threshold = 10);
void score_long_safe();