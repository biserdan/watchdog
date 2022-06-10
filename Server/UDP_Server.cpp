/**
 * @file UDP_Server.cpp
 * @author Client application main source file
 * @brief UDP_Server source file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "UDP_Server.h"

/**
 * @brief Construct a new udp server::udp server object
 * 
 * @param server_port Sets the port for incomming messages
 */
UDP_Server::UDP_Server(int server_port) {
    UDP_Server::create_socket();
    UDP_Server::bind_socket(server_port);
}

/**
 * @brief Construct a new udp server::udp server object
 * 
 * @param server_port Sets the port for incomming messages
 * @param timeout Sets the timeout in usec for incomming messages
 */
UDP_Server::UDP_Server(int server_port, int timeout) {
    UDP_Server::create_socket();
    UDP_Server::set_timeout(timeout);
    UDP_Server::bind_socket(server_port);
}

/**
 * @brief Destroy the udp server::udp server object
 * 
 */
UDP_Server::~UDP_Server() {
}

/**
 * @brief Creates the socket and terminates program in the event of an error
 * 
 */
void UDP_Server::create_socket() {
    /* create socket */
    s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        printf("Can't open socket ...(%s)\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Sets the timeout in usec for incomming messages
 * 
 * @param timeout Contains the timeout in usec
 */
void UDP_Server::set_timeout(int timeout) {
    struct timeval tv;

    /* set timeout */
    tv.tv_sec = TIMEOUT_SEC;
    tv.tv_usec = timeout;
    setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
}

/**
 * @brief Binds the socket with the given port
 * 
 * @param server_port Contains the desired port for incomming messages
 */
void UDP_Server::bind_socket(int server_port) {
    const int y = 1;

    /* bind local server port */
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(server_port);
    setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int));

    rc = bind(s, (struct sockaddr*)&servAddr,
              sizeof(servAddr));

    if (rc < 0) {
        printf("Can't bind port %d (%s)\n", server_port, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Receives incomming messages from client
 * 
 * @param puffer Contains the buffer pointer for incomming messages
 * @param length Contains the buffer size
 * @return int Returns error code for recvfrom(...)
 */
int UDP_Server::receive(char *puffer, int length) {
    int n;
    socklen_t len = sizeof(cliAddr);
    n = recvfrom(s, puffer, length, 0,
                 (struct sockaddr*)&cliAddr, &len);

//    if (n >= 0) {
//        printf("Data received from %s:Port%u : %s \n",
//               inet_ntoa(cliAddr.sin_addr),
//               ntohs(cliAddr.sin_port), puffer);
//    }

    return n;
}