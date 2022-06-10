import Jetson.GPIO as GPIO

PIN_STEER = 33
PIN_SPEED = 32
PWM_FREQ = 400

STEER_MIDDLE = 60.0
STEER_MOST_RIGHT = 22.5
STEER_MOST_LEFT = 67.5
SPEED_MIDDLE = 60.0
SPEED_MOST_BACKWARD = 22.5
SPEED_MOST_FORWARD = 67.5

steer_state = STEER_MIDDLE
speed_state = SPEED_MIDDLE

def stop():
    steer_pwm.stop()
    speed_pwm.stop()
    GPIO.cleanup()

# GPIO pin setup
GPIO.setmode(GPIO.BOARD)
GPIO.setup(PIN_STEER, GPIO.OUT)
GPIO.setup(PIN_SPEED, GPIO.OUT)
steer_pwm = GPIO.PWM(PIN_STEER, PWM_FREQ)
speed_pwm = GPIO.PWM(PIN_SPEED, PWM_FREQ)

# auto-calibrate the esc, wait for the beep feedback
steer_pwm.start(STEER_MIDDLE)
speed_pwm.start(SPEED_MIDDLE)

while(True):
    print("w:fw\ts:bw\ta:left\td:right\tSPACE:stop\tz:close\n")
    cmd = input("cmd: ")
    if cmd == "w":
        if speed_state + 0.1 < SPEED_MOST_FORWARD:
            speed_state += 0.1
    elif cmd == "s":
        if speed_state - 0.1 > SPEED_MOST_BACKWARD:
            speed_state -= 0.1
    elif cmd == "a":
        if steer_state + 0.1 < STEER_MOST_LEFT:
            steer_state += 0.1
    elif cmd == "d":
        if steer_state - 0.1 > STEER_MOST_RIGHT:
            steer_state -= 0.1
    elif cmd == " ":
        steer_state = STEER_MIDDLE
        speed_state = SPEED_MIDDLE
    elif cmd == "z":
        steer_state = STEER_MIDDLE
        speed_state = SPEED_MIDDLE
        stop()
        print("program stopped")
        break
    steer_pwm.ChangeDutyCycle(steer_state)
    speed_pwm.ChangeDutyCycle(speed_state)
    print(f"steer position: {steer_state}")
    print(f"speed position: {speed_state}")
exit()




