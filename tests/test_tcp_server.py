import socket
import sys

# GPIO part, todo
import Jetson.GPIO as GPIO

PIN_STEER = 33
PIN_SPEED = 32
PWM_FREQ = 200

STEER_MIDDLE = 30
STEER_MOST_RIGHT = 15
STEER_MOST_LEFT = 45
SPEED_MIDDLE = 30
SPEED_MOST_FORWARD = 30
SPEED_MOST_BACKWARD = 30

steer_state = STEER_MIDDLE
speed_state = SPEED_MIDDLE

# Initialise PWM frequencies
# GPIO.setmode(GPIO.BOARD)
# GPIO.setup(PIN_STEER, GPIO.OUT)
# GPIO.setup(PIN_SPEED, GPIO.OUT)
# steer_pwm = GPIO.PWM(PIN_STEER, PWM_FREQ)
# speed_pwm = GPIO.PWM(PIN_SPEED, PWM_FREQ)
# steer_pwm.start(STEER_MIDDLE)
# speed_pwm.start(SPEED_MIDDLE)

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Bind the socket to the port, visible to the outside
server_address = ('', 10000)
print(f'starting up on {server_address} port {server_address}', file = sys.stderr)
sock.bind(server_address)

# Listen for incoming connections
sock.listen(1)

while True:
    # Wait for a connection
    print('waiting for a connection', file = sys.stderr)
    connection, client_address = sock.accept()

    try:
        print(f'connection from {client_address}', file = sys.stderr)

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(16)
            print(f'received {data}', file=sys.stderr)
            if data:
                print('sending data back to the client', file=sys.stderr)
                connection.sendall(data)
                if data.decode() == 'right':
                    if steer_state + 1 > STEER_MOST_RIGHT:
                        steer_state += 1
                    # steer_pwm.ChangeDutyCycle(steer_state)
                elif data.decode() == 'left':
                    if steer_state - 1 > STEER_MOST_LEFT:
                        steer_state -= 1
                    # steer_pwm.ChangeDutyCycle(steer_state)
                elif data.decode() == 'forward':
                    if speed_state - 1 > SPEED_MOST_FORWARD:
                        speed_state -= 1
                    # speed_pwm.ChangeDutyCycle(speed_state)
                elif data.decode() == 'backward':
                    if speed_state - 1 > SPEED_MOST_BACKWARD:
                        speed_state -= 1
                    # speed_pwm.ChangeDutyCycle(speed_state)
                elif data.decode() == 'stop':
                    foo = 0
                    # steer_pwm.ChangeDutyCycle(STEER_MIDDLE)
                    # speed_pwm.ChangeDutyCycle(SPEED_MIDDLE)
                elif data.decode() == 'exit':
                    # steer_pwm.stop()
                    # speed_pwm.stop()
                    # GPIO.cleanup()
                    connection.close()
                    print("connection closed")
                    exit()

                print(f"steer position: {steer_state}")
                print(f"speed position: {speed_state}")
            else:
                print(f'no more data from {client_address}', file=sys.stderr)
                break

    finally:
        # Clean up the connection
        connection.close()