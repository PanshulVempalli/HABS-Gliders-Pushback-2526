'''— Autonomous routine and mechanism helpers
Team: HABS Gliders 34071B 
Creator : Panshul Vempalli
Team Website : https://habs-gliders-34071b.vercel.app/
Contact Info : panshulvempalli@gmail.com
Game: VEX Push Back 2025/26'''

'''push to github commands :git add .
git commit -m "describe what you changed"
git push'''

'''I recommend using the VEXcode V5 Text Editor for this code, but you can use any text editor you like. 
Just make sure to save it as main.py and upload it to your V5 Brain.'''

from vex import *

# ---------------- BRAIN ----------------
brain = Brain()

# ---------------- DRIVE MOTORS ----------------
# LEFT SIDE
left_front  = Motor(Ports.PORT1, GearSetting.RATIO_18_1, False)
left_middle = Motor(Ports.PORT2, GearSetting.RATIO_18_1, False)
left_back   = Motor(Ports.PORT3, GearSetting.RATIO_6_1,  False)

# RIGHT SIDE
right_front  = Motor(Ports.PORT4, GearSetting.RATIO_18_1, True)
right_middle = Motor(Ports.PORT5, GearSetting.RATIO_18_1, True)
right_back   = Motor(Ports.PORT6, GearSetting.RATIO_6_1,  True)

# Intake
intake = Motor(Ports.PORT10, GearSetting.RATIO_18_1, False)

# Outtake
outtake = Motor(Ports.PORT8, GearSetting.RATIO_18_1, False)

# Match Loader
match_loader = DigitalOut(brain.three_wire_port.g)

# Wing
wing = DigitalOut(brain.three_wire_port.h)

drive_motors = [
    left_front, left_middle, left_back,
    right_front, right_middle, right_back
]

# ---------------- DRIVE HELPERS ----------------
def stop_drive():
    for m in drive_motors:
        m.stop()

def drive_forward(time_sec, speed=60):
    for m in drive_motors:
        m.set_velocity(speed, PERCENT)
        m.spin(FORWARD)
    wait(time_sec, SECONDS)
    stop_drive()

def drive_backward(time_sec, speed=60):
    for m in drive_motors:
        m.set_velocity(speed, PERCENT)
        m.spin(REVERSE)
    wait(time_sec, SECONDS)
    stop_drive()

def turn_left(time_sec, speed=35):
    for m in drive_motors:
        m.set_stopping(BRAKE)
    for m in [left_front, left_middle, left_back]:
        m.set_velocity(speed, PERCENT)
        m.spin(REVERSE)
    for m in [right_front, right_middle, right_back]:
        m.set_velocity(speed, PERCENT)
        m.spin(FORWARD)
    wait(time_sec, SECONDS)
    stop_drive()

def turn_right(time_sec, speed=35):
    for m in drive_motors:
        m.set_stopping(BRAKE)
    for m in [left_front, left_middle, left_back]:
        m.set_velocity(speed, PERCENT)
        m.spin(FORWARD)
    for m in [right_front, right_middle, right_back]:
        m.set_velocity(speed, PERCENT)
        m.spin(REVERSE)
    wait(time_sec, SECONDS)
    stop_drive()

def score(time_sec):
    outtake.set_velocity(100, PERCENT)
    outtake.spin(FORWARD)
    wait(time_sec, SECONDS)
    outtake.stop()

def intake_on():
    intake.set_velocity(100, PERCENT)
    intake.spin(FORWARD)

def intake_off():
    intake.stop()

# ---------------- AUTON ----------------
def autonomous():

    # Move forward
    drive_forward(0.8)
    wait(0.2, SECONDS)

    # Slightly reduced first 90°
    turn_left(1.05)   # reduced slightly from 1.1
    wait(0.3, SECONDS)

    # Reverse into long goal
    drive_backward(1.2)
    wait(0.3, SECONDS)

    # Score preload
    score(1.0)
    wait(0.4, SECONDS)

    # Drive forward into match loader
    drive_forward(1.6)
    wait(0.3, SECONDS)

    # Fire piston
    match_loader.set(True)
    wait(0.4, SECONDS)
    match_loader.set(False)
    wait(0.3, SECONDS)

    # Intake blocks
    intake_on()
    wait(1.0, SECONDS)
    intake_off()
    wait(0.3, SECONDS)

    # Reverse back into long goal
    drive_backward(1.8)
    wait(0.3, SECONDS)

    # Outtake
    score(1.5)

    # ==========================
    # WING SECTION
    # ==========================

    drive_forward(0.4)
    wait(0.2, SECONDS)

    wing.set(True)
    wait(0.4, SECONDS)

    turn_left(1.1)
    wait(0.3, SECONDS)

    drive_forward(0.5)
    wait(0.2, SECONDS)

    turn_right(1.1)
    wait(0.3, SECONDS)

    # FINAL PUSH (now REVERSE as requested)
    drive_backward(0.8)

# ---------------- RUN ----------------
autonomous()