/**
 * @file UDP_Server.h
 * @author Client application main source file
 * @brief UDP_Server header file
 * @version 0.1
 * @date 2022-06-10
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef BA_UDP_SERVER_H
#define BA_UDP_SERVER_H


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define TIMEOUT_SEC 0                   //!< Timeout for incomming messages in seconds

/**
 * @brief Class for setting up the UDP server socket
 * 
 */
class UDP_Server {
public:
    UDP_Server(int server_port);
    UDP_Server(int server_port, int timeout);
    ~UDP_Server();
    int receive(char *puffer, int length);

    struct sockaddr_in cliAddr;

private:
    void create_socket();
    void set_timeout(int timeout);
    void bind_socket(int server_port);

    struct sockaddr_in servAddr;
    int s;
    int rc;

};


#endif //BA_UDP_SERVER_H
