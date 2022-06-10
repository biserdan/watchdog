
PIN_STEER = 33
PIN_SPEED = 32
PWM_FREQ = 400

STEER_MIDDLE = 60.0
STEER_MOST_RIGHT = 22.5
STEER_MOST_LEFT = 67.5
SPEED_MIDDLE = 30.0
SPEED_MOST_BACKWARD = 15.0
SPEED_MOST_FORWARD = 45.0

steer_state = STEER_MIDDLE
speed_state = SPEED_MIDDLE

def stop():
    steer_pwm.stop()
    speed_pwm.stop()
    GPIO.cleanup()



while(True):
    print("press keys for following commands:")
    print("w:fw\ts:bw\ta:left\td:right\tSPACE:stop\tz:close")
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
        # stop()
        print("program stopped")
        break
    print(f"steer position: {steer_state}")
    print(f"speed position: {speed_state}\n")
exit()




