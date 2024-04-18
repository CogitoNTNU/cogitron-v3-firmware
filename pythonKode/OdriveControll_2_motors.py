import odrive
from odrive.enums import *
import time
import pygame
import threading

pygame.init()
screen = pygame.display.set_mode((1280, 720))
oDrive = odrive.find_any()


def configure_motors():
    oDrive.config.enable_brake_resistor
    oDrive.config.brake_resistance = 0.55
    oDrive.config.dc_max_negative_current = -0.01
    # motor0
    oDrive.axis0.motor.config.current_lim = 10
    oDrive.axis0.controller.config.vel_limit = 2
    oDrive.axis0.motor.config.torque_constant = 8.27 / 270
    oDrive.axis0.motor.config.pole_pairs = 7
    oDrive.axis0.motor.config.motor_type = 0
    oDrive.axis0.controller.config.vel_limit = 4
    #motor1
    oDrive.axis1.motor.config.current_lim = 10
    oDrive.axis1.controller.config.vel_limit = 2
    oDrive.axis1.motor.config.torque_constant = 8.27 / 270
    oDrive.axis1.motor.config.pole_pairs = 7
    oDrive.axis1.motor.config.motor_type = 0
    oDrive.axis1.controller.config.vel_limit = 4
    
    
    
def calibrate_motors():
    #Motor 0
    oDrive.axis0.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE
    oDrive.axis0.controller.config.circular_setpoints = True
    while oDrive.axis1.current_state != AXIS_STATE_IDLE:
        time.sleep(0.1)
    if odrive.axis0.encoder.config.pre_calibrated == False:
        print("MOTOR 0 CALIBRATION ERROR!")
    
    #Motor 1
    oDrive.axis1.requested_state = AXIS_STATE_FULL_CALIBRATION_SEQUENCE
    oDrive.axis1.controller.config.circular_setpoints = True
    while oDrive.axis1.current_state != AXIS_STATE_IDLE:
        time.sleep(0.1)
    
    
    if odrive.axis1.encoder.config.pre_calibrated == False:
        print("MOTOR 1 CALIBRATION ERROR!")
    else:
        oDrive.save_configuration()
        oDrive.reboot()
        oDrive = odrive.find_any()

def velocity_conrol():
    oDrive.axis0.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL
    #oDrive0.axis0.requested_state = AxisState.CLOSED_LOOP_CONTROL
    oDrive.axis0.controller.config.circular_setpoints = True
    oDrive.axis0.controller.config.control_mode = CONTROL_MODE_VELOCITY_CONTROL
    #oDrive0.axis0.controller.config.control_mode = ControlMode.VELOCITY_CONTROL

    oDrive.axis1.requested_state = AXIS_STATE_CLOSED_LOOP_CONTROL
    #oDrive0.axis0.requested_state = AxisState.CLOSED_LOOP_CONTROL
    oDrive.axis1.controller.config.circular_setpoints = True
    oDrive.axis1.controller.config.control_mode = CONTROL_MODE_VELOCITY_CONTROL
    #oDrive0.axis0.controller.config.control_mode = ControlMode.VELOCITY_CONTROL
    
def controllMotor0():
    run = True
    vel = 0
    while run == True:
        events = pygame.event.get()
        for event in events:
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_UP:
                    vel1 += 1
                    print("Motor 0 vel: ",vel)
                if event.key == pygame.K_DOWN:
                    vel1 -= 1
                    print("Motor 0 vel: ",vel)
                if event.key == pygame.K_q:
                    run = False
        oDrive.axis0.controller.input_vel = vel

def controllMotor1():
    run = True
    vel = 0
    while run == True:
        events = pygame.event.get()
        for event in events:
            if event.type == pygame.KEYDOWN:
                if event.key == pygame.K_RIGHT:
                    vel1 += 1
                    print("Motor 1 vel: ",vel)
                if event.key == pygame.K_LEFT:
                    vel1 -= 1
                    print("Motor 1 vel: ",vel)
                if event.key == pygame.K_q:
                    run = False
        oDrive.axis1.controller.input_vel = vel


def selectControlMode():
    inputMode = input("1: velocity_control")
    if inputMode == '1':
        print("velocity control selected")
        velocity_conrol()
    else:
        print("You dont have a choice! velocity control selected")
        velocity_conrol()
     
def runMotors():
    print("Controll motor 0 with UP and DOWN keys, Controll motor 1 with LEFT and RIGHT keys and press Q to quit")
    t0 = threading.Thread(controllMotor0())
    t1 = threading.Thread(controllMotor1())

    t0.start()
    t1.start()

    t0.join()
    t1.join()

def main():
    #konfigurer motorene
    configure_motors()
    calibrate_motors()
    velocity_conrol()
    print("Motors are calibrated and ready to run! (hopefully*)")

    #velg inputmode
    while True:
        inputMode = input("1: Calibrate, 2: Select Control Mode, 3: Run Motors")
        if inputMode == '1':
            calibrate_motors()
        elif inputMode == '2':
            selectControlMode()
        elif inputMode == '3':
            runMotors()

main()