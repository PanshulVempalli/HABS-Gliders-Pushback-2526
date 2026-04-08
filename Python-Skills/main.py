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

#region VEXcode Generated Robot Configuration
from vex import *
import urandom
import math

# Brain should be defined by default
brain=Brain()

# Robot configuration code


# wait for rotation sensor to fully initialize
wait(30, MSEC)


# Make random actually random
def initializeRandomSeed():
    wait(100, MSEC)
    random = brain.battery.voltage(MV) + brain.battery.current(CurrentUnits.AMP) * 100 + brain.timer.system_high_res()
    urandom.seed(int(random))
      
# Set random seed 
initializeRandomSeed()


def play_vexcode_sound(sound_name):
    # Helper to make playing sounds from the V5 in VEXcode easier and
    # keeps the code cleaner by making it clear what is happening.
    print("VEXPlaySound:" + sound_name)
    wait(5, MSEC)

# add a small delay to make sure we don't print in the middle of the REPL header
wait(200, MSEC)
# clear the console to make sure we don't have the REPL in the console
print("\033[2J")

#endregion VEXcode Generated Robot Configuration
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

    
def drive_forwardslow(time_sec, speed=30):
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
    outtake.spin(REVERSE)
    wait(time_sec, SECONDS)
    outtake.stop()

def intake_on():
    intake.set_velocity(100, PERCENT)
    intake.spin(FORWARD)

def intake_off():
    intake.stop()

# ---------------- SKILLS AUTON ----------------
def autonomous():

    # ---- SAME BEGINNING AS PRE MATCH ----
    match_loader.set(True)
    
    drive_forward(1.8)
    wait(0.2, SECONDS)

    turn_left(1.0)
    wait(0.3, SECONDS)

    drive_backward(0.9)
    wait(0.3, SECONDS)

    score(5)
    wait(0.4, SECONDS)
    match_loader.set(False)

    drive_forward(1.3)
    wait(0.3, SECONDS)

    drive_forwardslow(0.9)
    wait(0.3, SECONDS)


    match_loader.set(False)
    wait(0.4, SECONDS)
    match_loader.set(False)
    wait(0.3, SECONDS)

    intake_on()
    wait(7.0, SECONDS)
    intake_off()    
    wait(0.3, SECONDS)

    drive_backward(1.2)
    wait(0.3, SECONDS)

    score(5)

    # =====================================================
    # SKILLS PARK ROUTINE (intake facing WALL)
    # =====================================================

    #  Move slightly forward
    drive_forward(0.4)
    wait(0.2, SECONDS)

    #  90° LEFT
    turn_left(1.03)
    wait(0.3, SECONDS)

    #  Move slightly forward
    drive_forward(1.5)
    wait(0.2, SECONDS)

    #  90° RIGHT
    turn_left(1.0)
    wait(0.3, SECONDS)

    match_loader.set(True)

    # Drive forward to wall
    drive_backward(1.2)
    wait(0.3, SECONDS)

    # 90° RIGHT
    turn_left(1) 
    
    wait(0.3, SECONDS)

  

    drive_backward(2.0)

# ---------------- RUN ----------------
autonomous()
