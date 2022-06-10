import socket
import time
import threading

UDP_IP = "ba22openpilot"
UDP_PORT = 10000
WD_PORT = 10001

def kick_wd():
    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:# UDP

        print("Kicker started")

        while True:
            message = "x"
            sock.sendto(message.encode(), (UDP_IP, WD_PORT))
            time.sleep(0.001)


def main():
    t1 = threading.Thread(target=kick_wd, args=(), daemon=True)
    # t1.start()

    print("UDP target IP: %s" % UDP_IP)
    print("UDP target port: %s" % UDP_PORT)

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sock:# UDP

        print("Connection started")

        while True:
            message = input("Enter Command wsad z:")
            sock.sendto(message.encode(), (UDP_IP, UDP_PORT))

            if message == "z":
                #todo, not working
                break

    print("Connection closed")

if __name__ == "__main__":
    main()