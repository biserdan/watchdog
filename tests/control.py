# GPIO part
import Jetson.GPIO as GPIO
import time

PIN_STEER = 33
PIN_SPEED = 32
PWM_FREQ = 400

STEER_MIDDLE = 56
STEER_MOST_RIGHT = 30
STEER_MOST_LEFT = 82
SPEED_MIDDLE = 60
SPEED_MOST_FORWARD = 75
SPEED_MOST_BACKWARD = 45


class Control:
    def __init__(self):
        self.steer_state = STEER_MIDDLE
        self.speed_state = SPEED_MIDDLE

        # Initialise PWM frequencies
        try:
            GPIO.cleanup()
        finally:
            GPIO.setmode(GPIO.BOARD)
            GPIO.setup(PIN_STEER, GPIO.OUT)
            GPIO.setup(PIN_SPEED, GPIO.OUT)
            self.steer_pwm = GPIO.PWM(PIN_STEER, PWM_FREQ)
            self.speed_pwm = GPIO.PWM(PIN_SPEED, PWM_FREQ)

    def setup(self):
        self.steer_pwm.start(STEER_MIDDLE)
        self.speed_pwm.start(SPEED_MIDDLE)

    def process(self, commmand):
        if commmand == 'd': #right
            if self.steer_state - 1 > STEER_MOST_RIGHT:
                self.steer_state -= 1
            self.steer_pwm.ChangeDutyCycle(self.steer_state)
        elif commmand == 'a': #left
            if self.steer_state + 1 < STEER_MOST_LEFT:
                self.steer_state += 1
            self.steer_pwm.ChangeDutyCycle(self.steer_state)
        elif commmand == 'w': #forward
            if self.speed_state + 1 < SPEED_MOST_FORWARD:
                self.speed_state += 1
            self.speed_pwm.ChangeDutyCycle(self.speed_state)
        elif commmand == 's': #backward
            if self.speed_state - 1 > SPEED_MOST_BACKWARD:
                self.speed_state -= 1
            self.speed_pwm.ChangeDutyCycle(self.speed_state)
        elif commmand == 'x': #stop
            self.steer_pwm.ChangeDutyCycle(STEER_MIDDLE)
            self.speed_pwm.ChangeDutyCycle(SPEED_MIDDLE)
            self.steer_state = STEER_MIDDLE
            self.speed_state = SPEED_MIDDLE
        elif commmand == 'z': #exit
            self.steer_pwm.ChangeDutyCycle(STEER_MIDDLE)
            self.speed_pwm.ChangeDutyCycle(SPEED_MIDDLE)
            time.sleep(1)
            self.steer_pwm.stop()
            self.speed_pwm.stop()
            GPIO.cleanup()
        else:
            print("command not supported")

        print(f"steer position: {self.steer_state}")
        print(f"speed position: {self.speed_state}")
