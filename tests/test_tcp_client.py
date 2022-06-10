import socket
import sys
import keyboard

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Connect the socket to the port where the server is listening
server_address = ('ba22openpilot', 10000)
print(f'starting up on {server_address[0]} port {server_address[1]}', file = sys.stderr)
sock.connect(server_address)

try:

    while True:
        message = ""
        if keyboard.read_key() == "w":
            message = 'forward'
            print("Go forward")
        elif keyboard.read_key() == "s":
            message = 'back'
            print("Go backward")
        elif keyboard.read_key() == "a":
            message = 'left'
            print("Go left")
        elif keyboard.read_key() == "d":
            message = 'right'
            print("Go right")
        elif keyboard.read_key() == " ":
            message = 'stop'
            print("Stop")
        elif keyboard.read_key() == "z":
            message = 'exit'

        if not message:
            # Send data
            # print(f'sending {message}', file=sys.stderr)
            sock.sendall(message.encode())

            # Look for the response
            amount_received = 0
            amount_expected = len(message)

            while amount_received < amount_expected:
                data = sock.recv(16)
                amount_received += len(data)
                print(f'received {data}', file=sys.stderr)
    
        if message == "exit":
            print('closing socket', file=sys.stderr)
            sock.close()
            exit()

finally:
    print('closing socket', file=sys.stderr)
    sock.close()