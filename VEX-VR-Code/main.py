#region VEXcode Generated Robot Configuration
import math
import random
from vexcode_vrc import *
from vexcode_vrc.events import get_Task_func

brain = Brain()

conveyor_motor = Motor("ConveyorMotor", 3)
intake_motor    = Motor("IntakeMotor", 4)
bumper          = Bumper("Bumper", 5)
ai_vision       = AiVision("aiVision", 6)
gps             = GPS("GPS", 7)
optical         = Optical("Optical", 8)
distance        = Distance("Distance", 9)
drivetrain      = Drivetrain("drivetrain", 0)

# AI Vision Enum
class GameElements:
   BLUE_BLOCK = 0
   RED_BLOCK  = 1


# Color Helper
def convert_color_to_string(col):
   if col == RED: return "red"
   if col == GREEN: return "green"
   if col == BLUE: return "blue"
   if col == WHITE: return "white"
   if col == YELLOW: return "yellow"
   if col == ORANGE: return "orange"
   if col == PURPLE: return "purple"
   if col == CYAN: return "cyan"
   if col == RED_VIOLET: return "red_violet"
   if col == VIOLET: return "violet"
   if col == BLUE_VIOLET: return "blue_violet"
   if col == BLUE_GREEN: return "blue_green"
   if col == YELLOW_GREEN: return "yellow_green"
   if col == YELLOW_ORANGE: return "yellow_orange"
   if col == RED_ORANGE: return "red_orange"
   if col == BLACK: return "black"
   if col == TRANSPARENT: return "transparent"
   return ""

#endregion



# ---------------- MOTOR SETUP ----------------
def motorSetup():
   drivetrain.set_drive_velocity(100, PERCENT)
   drivetrain.set_turn_velocity(100, PERCENT)
   conveyor_motor.set_velocity(75, PERCENT)
   intake_motor.set_velocity(100, PERCENT)
   intake_motor.spin(FORWARD)


# ---------------- POSITION DATA ----------------
class posData:
   def __init__(self, x, y, theta=0):
       self.x = x
       self.y = y
       self.theta = theta


# Heading Offset
headingOffset = gps.heading() - drivetrain.heading(DEGREES)
headingOffset = (headingOffset + 180) % 360 - 180


# ---------------- ROBOT CLASS ----------------
class heroBot:

   def __init__(self):
       self.position   = posData(0, 0, 0)
       self.dx = self.dy = self.dis = self.target_ang = 0

   def printReadings(self):
       while True:
           brain.screen.clear_screen()
           brain.screen.print(
               f"X: {gps.x_position(MM)} "
               f"Y: {gps.y_position(MM)} "
               f"H: {gps.heading()} "
               f"DH: {drivetrain.heading(DEGREES)}"
           )
           wait(10, MSEC)

   def updatePos(self, x, y):
       self.position = posData(
           gps.x_position(MM),
           gps.y_position(MM),
           drivetrain.heading(DEGREES)
       )

       self.dx = x - self.position.x
       self.dy = y - self.position.y
       self.dis = math.sqrt(self.dx ** 2 + self.dy ** 2)
       self.target_ang = (math.degrees(math.atan2(self.dx, self.dy))) % 360

   def turnTo(self, angle, waitt=True):
       turnAmount = (angle - headingOffset) % 360
       drivetrain.turn_to_heading(turnAmount, DEGREES, waitt)

   def moveTo(self, x, y, error=0):
       self.updatePos(x, y)
       turnAmount = (self.target_ang - headingOffset) % 360
       drivetrain.turn_to_heading(turnAmount, DEGREES)
       drivetrain.drive_for(FORWARD, self.dis - error, MM)


bot = heroBot()


# ---------------- GAME FUNCTIONS ----------------
def score(two=False):
   conveyor_motor.set_velocity(100, PERCENT)
   conveyor_motor.spin(FORWARD)

   if not two:
       wait(1.26, SECONDS)
   else:
       wait(0.90, SECONDS)

   conveyor_motor.set_velocity(25, PERCENT)
   conveyor_motor.stop()


def pick():
   conveyor_motor.set_velocity(100, PERCENT)
   conveyor_motor.spin(FORWARD)
   wait(1, SECONDS)
   drivetrain.drive_for(REVERSE, 100, MM)
   conveyor_motor.stop()


def scorePick():
   drivetrain.drive_for(REVERSE, 547, MM)
   score()
   drivetrain.drive_for(FORWARD, 646, MM)
   pick()


# ---------------- MAIN ROUTE ----------------
def main():

   motorSetup()
   conveyor_motor.spin(FORWARD)

   # --- Mid Block Score ---
   bot.moveTo(-600, -650)
   intake_motor.stop()
   drivetrain.drive_for(FORWARD, 150, MM)

   conveyor_motor.set_velocity(4, PERCENT)
   bot.turnTo(-135)
   conveyor_motor.stop()

   drivetrain.drive_for(REVERSE, 340, MM)
   score(True)
   intake_motor.spin(FORWARD)

   # --- Bottom Left Loader ---
   drivetrain.drive_for(FORWARD, 1125, MM)
   bot.turnTo(-90)
   drivetrain.drive_for(FORWARD, 140, MM)
   pick()

   # --- Score Reds ---
   bot.turnTo(-135)
   drivetrain.drive_for(REVERSE, 1130, MM)
   score()

   # --- Mid Collection ---
   conveyor_motor.spin(FORWARD)
   bot.turnTo(55)
   bot.turnTo(-155)

   conveyor_motor.set_velocity(3, PERCENT)
   drivetrain.drive_for(FORWARD, 900, MM)
   conveyor_motor.stop()

   bot.turnTo(-90)
   drivetrain.drive_for(REVERSE, 125, MM)
   score(True)

   # --- Bottom Left Blues ---
   drivetrain.drive_for(FORWARD, 640, MM)
   pick()

   # --- Long Goal Score ---
   bot.moveTo(-600, -1550)
   bot.moveTo(900, -1600)
   bot.moveTo(1100, -1200)

   bot.turnTo(90)
   drivetrain.drive_for(REVERSE, 190, MM)
   score()

   # --- Bottom Right Loader ---
   drivetrain.drive_for(FORWARD, 645, MM)
   pick()
   scorePick()
   drivetrain.drive_for(REVERSE, 545, MM)
   score()

   # --- Clear & Collect ---
   bot.moveTo(1700, -500, 115)
   bot.turnTo(-90)
   drivetrain.drive_for(REVERSE, 180, MM)
   bot.turnTo(5)

   drivetrain.set_drive_velocity(35, PERCENT)
   conveyor_motor.set_velocity(55, PERCENT)
   conveyor_motor.spin(FORWARD)
   drivetrain.drive_for(FORWARD, 545, MM)

   conveyor_motor.stop()
   intake_motor.stop()
   drivetrain.set_drive_velocity(100, PERCENT)

   # --- Mid Goal ---
   bot.moveTo(450, 750, 50)
   bot.turnTo(43)
   drivetrain.drive_for(REVERSE, 150, MM)
   intake_motor.spin(FORWARD)
   score()

   # --- Top Right Loader ---
   conveyor_motor.spin(FORWARD)
   conveyor_motor.set_velocity(60, PERCENT)

   drivetrain.drive_for(FORWARD, 1070, MM)
   bot.turnTo(90)
   drivetrain.drive_for(REVERSE, 420, MM)
   score(True)

   drivetrain.drive_for(FORWARD, 646, MM)
   pick()
   scorePick()
   drivetrain.drive_for(REVERSE, 50, MM)

   # --- Top Left Long Goal ---
   bot.moveTo(900, 1500)
   bot.moveTo(-900, 1750)
   bot.moveTo(-1000, 1200, 10)

   bot.turnTo(-90)
   drivetrain.drive_for(REVERSE, 75, MM)
   score()

   drivetrain.drive_for(FORWARD, 637, MM)
   pick()
   scorePick()
   drivetrain.drive_for(REVERSE, 525, MM)
   score()

   # --- Final Park ---
   bot.moveTo(-1630, 450, 125)
   bot.turnTo(90)
   drivetrain.drive_for(REVERSE, 272, MM)

   bot.turnTo(-175)
   drivetrain.set_drive_velocity(45, PERCENT)
   conveyor_motor.set_velocity(100, PERCENT)
   conveyor_motor.spin(FORWARD)

   drivetrain.drive_for(FORWARD, 520, MM)
   bot.turnTo(-150, False)

   wait(0.10, SECONDS)

   while True:
       bumper.pressed(stop_project())
       wait(1, MSEC)


# Threads
vr_thread(main)
vr_thread(bot.printReadings)
