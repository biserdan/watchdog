import socket
import control
import threading
import select

UDP_IP = ""
UDP_PORT = 10000
WD_PORT = 10001
TIMEOUT = 0.01

def watchdog():

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as server:# UDP
        server.setblocking(0)
        server.bind((UDP_IP, WD_PORT))
        print("watchdog started...")

        server.settimeout(TIMEOUT)

        while True:
            try:
                server.recv(1024)
            except socket.timeout as e:
                print(e)


def main():
    ctrl = control.Control()
    ctrl.setup()

    t1 = threading.Thread(target=watchdog, args=(), daemon=True)
    # t1.start()

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:# UDP
        sock.bind((UDP_IP, UDP_PORT))
        print("server started...")

        while True:
            data, addr = sock.recvfrom(1024) # buffer size is 1024 bytes
            command = data.decode()
            print("command: %s" % command)

            ctrl.process(command)

            if command == "z":
                break

    print("server stopped")

if __name__ == "__main__":
    main()